#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "usuario/Usuario.hpp"
#include "usuario/Sessao.hpp"
#include "contagem/Imagem.hpp"
#include "contagem/Contagem.hpp"
#include "contagem_gado/servico_contagem_gado.h"

using namespace contagem_gado;
namespace fs = std::filesystem;

// ---------- Apoio ----------

// Pasta temporaria onde os arquivos de teste sao criados.
static fs::path pasta() {
    return fs::temp_directory_path() / "bovicount_test_contagem";
}

static void limpar() {
    std::error_code erro;
    fs::remove_all(pasta(), erro);
    fs::create_directories(pasta());
}

static std::string caminho(const std::string& nome) {
    return (pasta() / nome).string();
}

static void escrever(const std::string& arquivo, const std::vector<unsigned char>& bytes) {
    std::ofstream saida(arquivo, std::ios::binary);
    saida.write(reinterpret_cast<const char*>(bytes.data()),
                static_cast<std::streamsize>(bytes.size()));
}

static void escreverBE32(std::vector<unsigned char>& v, std::uint32_t n) {
    v.push_back(static_cast<unsigned char>(n >> 24));
    v.push_back(static_cast<unsigned char>(n >> 16));
    v.push_back(static_cast<unsigned char>(n >> 8));
    v.push_back(static_cast<unsigned char>(n));
}

static void escreverBE16(std::vector<unsigned char>& v, std::uint16_t n) {
    v.push_back(static_cast<unsigned char>(n >> 8));
    v.push_back(static_cast<unsigned char>(n));
}

// Cria um arquivo com cabecalho PNG valido (assinatura + IHDR) com as dimensoes dadas.
static std::string criarPNG(const std::string& nome, std::uint32_t largura, std::uint32_t altura) {
    std::vector<unsigned char> b = {137, 80, 78, 71, 13, 10, 26, 10};
    escreverBE32(b, 13);
    b.push_back('I'); b.push_back('H'); b.push_back('D'); b.push_back('R');
    escreverBE32(b, largura);
    escreverBE32(b, altura);
    const unsigned char resto[] = {8, 2, 0, 0, 0, 0, 0, 0, 0};  // profundidade, cor, ... e CRC
    b.insert(b.end(), resto, resto + sizeof(resto));
    const std::string arquivo = caminho(nome);
    escrever(arquivo, b);
    return arquivo;
}

// Cria um arquivo com cabecalho JPEG valido (SOI + SOF0) com as dimensoes dadas.
static std::string criarJPEG(const std::string& nome, std::uint16_t largura, std::uint16_t altura) {
    std::vector<unsigned char> b = {0xFF, 0xD8, 0xFF, 0xC0};
    escreverBE16(b, 17);          // comprimento do segmento SOF0
    b.push_back(8);               // precisao
    escreverBE16(b, altura);
    escreverBE16(b, largura);
    const unsigned char resto[] = {3, 1, 0x22, 0, 2, 0x11, 1, 3, 0x11, 1};
    b.insert(b.end(), resto, resto + sizeof(resto));
    const std::string arquivo = caminho(nome);
    escrever(arquivo, b);
    return arquivo;
}

// Cria um arquivo qualquer com o texto dado (para testar cabecalhos invalidos).
static std::string criarTexto(const std::string& nome, const std::string& texto) {
    const std::string arquivo = caminho(nome);
    std::ofstream saida(arquivo, std::ios::binary);
    saida << texto;
    return arquivo;
}

// Motor de IA simulado: devolve sempre o mesmo numero de animais.
class MotorSimulado : public MotorVisao {
private:
    int animais;
    bool falha;

public:
    int chamadas;

    MotorSimulado(int animais, bool falha = false)
        : animais(animais), falha(falha), chamadas(0) {}

    bool detect(const Imagem& imagem, Contagem& contagem) {
        ++chamadas;
        Colecao<Retangulo> caixas;
        for (int i = 0; i < animais; ++i) {
            caixas.add(Retangulo(i, i, 10, 10));
        }
        contagem = Contagem(imagem, caixas, 0.5);  // mesmo falhando, o motor escreve na saida
        return !falha;
    }
};

static Sessao gestor()        { return Sessao(Usuario(1, "Gestor", 1)); }
static Sessao funcionario()   { return Sessao(Usuario(5, "Funcionário", 1)); }
static Sessao administrador() { return Sessao(Usuario(2, "Administrador", 1)); }

// Contagem de apoio, para checar que uma recusa nao altera a saida.
static Contagem contagemAnterior() {
    Colecao<Retangulo> caixas;
    caixas.add(Retangulo(1, 1, 5, 5));
    caixas.add(Retangulo(2, 2, 5, 5));
    caixas.add(Retangulo(3, 3, 5, 5));
    return Contagem(Imagem("anterior.jpg"), caixas, 9.0);
}

static bool inalterada(const Contagem& c) {
    return c.total() == 3 && c.image().path() == "anterior.jpg"
        && c.processingSeconds() == 9.0;
}

// ---------- Contar ----------

// Cenario 1: Funcionario envia a foto e o sistema marca 185 animais.
static void testeContagemFuncionario() {
    limpar();
    MotorSimulado motor(185);
    ServicoContagem servico(funcionario(), motor);

    const std::string foto = criarPNG("vistoria_20261006_pasto1.png", 1920, 1080);
    Contagem c;
    assert(servico.countByImage(Imagem(foto), c));
    assert(c.total() == 185);
    assert(c.boxes().size() == 185);
    assert(c.image().path() == foto);
    assert(c.processingSeconds() <= 30.0);
    assert(motor.chamadas == 1);
}

static void testeContagemGestorEAdministrador() {
    limpar();
    MotorSimulado motor(40);
    const std::string foto = criarPNG("foto.png", 800, 600);

    ServicoContagem comoGestor(gestor(), motor);
    ServicoContagem comoAdmin(administrador(), motor);

    Contagem a, b;
    assert(comoGestor.countByImage(Imagem(foto), a));
    assert(comoAdmin.countByImage(Imagem(foto), b));
    assert(a.total() == 40);
    assert(b.total() == 40);
}

// Formatos aceitos: jpeg, jpg e png (sem diferenciar maiusculas).
static void testeFormatosAceitos() {
    limpar();
    MotorSimulado motor(10);
    ServicoContagem servico(funcionario(), motor);

    Contagem c;
    assert(servico.countByImage(Imagem(criarPNG("a.png", 100, 100)), c));
    assert(servico.countByImage(Imagem(criarJPEG("b.jpg", 100, 100)), c));
    assert(servico.countByImage(Imagem(criarJPEG("c.jpeg", 100, 100)), c));
    assert(servico.countByImage(Imagem(criarJPEG("d.JPG", 100, 100)), c));
    assert(servico.countByImage(Imagem(criarPNG("e.PNG", 100, 100)), c));
    assert(motor.chamadas == 5);
}

// Cenario 3: pasto sem animais mostra total 0, um resultado valido.
static void testeContagemZero() {
    limpar();
    MotorSimulado motor(0);
    ServicoContagem servico(funcionario(), motor);

    Contagem c;
    assert(servico.countByImage(Imagem(criarPNG("vazio.png", 640, 480)), c));
    assert(c.total() == 0);
    assert(c.boxes().size() == 0);
}

// Uma nova contagem substitui o resultado anterior.
static void testeSubstituiContagemAnterior() {
    limpar();
    MotorSimulado motor(7);
    ServicoContagem servico(funcionario(), motor);

    Contagem c = contagemAnterior();
    assert(servico.countByImage(Imagem(criarPNG("nova.png", 640, 480)), c));
    assert(c.total() == 7);
}

// ---------- Recusas ----------

// Cenario 2: formato nao aceito, e o motor nem e chamado.
static void testeFormatoRecusado() {
    limpar();
    MotorSimulado motor(185);
    ServicoContagem servico(funcionario(), motor);

    Contagem c = contagemAnterior();
    assert(!servico.countByImage(Imagem(criarTexto("relatorio.pdf", "%PDF-1.4")), c));
    assert(!servico.countByImage(Imagem(criarTexto("foto.bmp", "BM")), c));
    assert(!servico.countByImage(Imagem(criarTexto("foto.gif", "GIF89a")), c));
    assert(!servico.countByImage(Imagem(criarTexto("semextensao", "x")), c));
    assert(inalterada(c));
    assert(motor.chamadas == 0);
}

static void testeArquivoInexistente() {
    limpar();
    MotorSimulado motor(185);
    ServicoContagem servico(funcionario(), motor);

    Contagem c = contagemAnterior();
    assert(!servico.countByImage(Imagem(caminho("nao_existe.png")), c));
    assert(!servico.countByImage(Imagem(""), c));
    assert(inalterada(c));
    assert(motor.chamadas == 0);
}

// Uma pasta nao e um arquivo de imagem, mesmo com nome terminado em .png.
static void testeDiretorioRecusado() {
    limpar();
    MotorSimulado motor(185);
    ServicoContagem servico(funcionario(), motor);

    fs::create_directories(pasta() / "pasta.png");
    Contagem c = contagemAnterior();
    assert(!servico.countByImage(Imagem(caminho("pasta.png")), c));
    assert(inalterada(c));
    assert(motor.chamadas == 0);
}

static void testePapelDesconhecido() {
    limpar();
    MotorSimulado motor(185);
    ServicoContagem intruso(Sessao(Usuario(7, "Visitante", 1)), motor);

    Contagem c = contagemAnterior();
    assert(!intruso.countByImage(Imagem(criarPNG("foto.png", 640, 480)), c));
    assert(inalterada(c));
    assert(motor.chamadas == 0);
}

// Limite de 4K: ate 3840x2160 (ou 2160x3840); acima disso e recusado.
static void testeLimite4K() {
    limpar();
    MotorSimulado motor(10);
    ServicoContagem servico(funcionario(), motor);
    Contagem c;

    // dentro do limite (inclusive exatamente 4K, em paisagem e retrato)
    assert(servico.countByImage(Imagem(criarPNG("uhd.png", 3840, 2160)), c));
    assert(servico.countByImage(Imagem(criarPNG("retrato.png", 2160, 3840)), c));
    assert(servico.countByImage(Imagem(criarJPEG("uhd.jpg", 3840, 2160)), c));
    assert(motor.chamadas == 3);

    // acima do limite
    Contagem intacta = contagemAnterior();
    assert(!servico.countByImage(Imagem(criarPNG("largura.png", 3841, 2160)), intacta));
    assert(!servico.countByImage(Imagem(criarPNG("altura.png", 3840, 2161)), intacta));
    assert(!servico.countByImage(Imagem(criarPNG("grande.png", 7680, 4320)), intacta));
    assert(!servico.countByImage(Imagem(criarJPEG("grande.jpg", 5000, 3000)), intacta));
    assert(inalterada(intacta));
    assert(motor.chamadas == 3);  // o motor nao recebeu nenhuma imagem acima de 4K
}

// Cabecalho invalido ou arquivo vazio: recusado sem chamar o motor.
static void testeCabecalhoInvalido() {
    limpar();
    MotorSimulado motor(185);
    ServicoContagem servico(funcionario(), motor);

    Contagem c = contagemAnterior();
    assert(!servico.countByImage(Imagem(criarTexto("falso.png", "isto nao e uma imagem")), c));
    assert(!servico.countByImage(Imagem(criarTexto("falso.jpg", "isto nao e uma imagem")), c));
    assert(!servico.countByImage(Imagem(criarTexto("vazio.png", "")), c));
    assert(!servico.countByImage(Imagem(criarTexto("vazio.jpg", "")), c));
    assert(!servico.countByImage(Imagem(criarTexto("curto.png", "\x89PNG")), c));
    assert(inalterada(c));
    assert(motor.chamadas == 0);
}

// Se o motor falhar, a Contagem de saida continua exatamente como estava.
static void testeFalhaDoMotor() {
    limpar();
    MotorSimulado motor(185, true);
    ServicoContagem servico(funcionario(), motor);

    Contagem c = contagemAnterior();
    assert(!servico.countByImage(Imagem(criarPNG("foto.png", 640, 480)), c));
    assert(motor.chamadas == 1);
    assert(inalterada(c));  // o motor escreveu 185 marcacoes, mas nada vazou para 'c'
}

int main() {
    std::cout << "===== TESTE DE CONTAGEM =====" << std::endl;
    testeContagemFuncionario();
    testeContagemGestorEAdministrador();
    testeFormatosAceitos();
    testeContagemZero();
    testeSubstituiContagemAnterior();
    testeFormatoRecusado();
    testeArquivoInexistente();
    testeDiretorioRecusado();
    testePapelDesconhecido();
    testeLimite4K();
    testeCabecalhoInvalido();
    testeFalhaDoMotor();
    limpar();
    std::error_code erro;
    fs::remove_all(pasta(), erro);
    std::cout << "  test_contagem: OK" << std::endl;
    std::cout << "=============================" << std::endl;
    return 0;
}