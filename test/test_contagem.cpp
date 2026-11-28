/**
 * @file test_contagem.cpp
 * @brief Testes do ServicoContagem (contar o gado a partir de uma foto).
 *
 * Os testes usam um motor de IA simulado (MotorSimulado) no lugar do motor real e imagens
 * criadas na hora, só com o cabeçalho PNG ou JPEG, em uma pasta temporária.
 * Cada função @c teste* cobre um cenário ou uma recusa e usa @c assert: a falha de qualquer
 * verificação aborta o programa. O programa imprime "test_contagem: OK" ao final se todos
 * os testes passarem.
 *
 * Os testes seguem os cenários de teste da seção 4.2 do projeto da API.
 * Compile com `make compile-test-contagem` e execute com `make run-test-contagem`.
 */
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

/**
 * @brief Pasta temporária onde os arquivos de teste são criados.
 *
 * @return Caminho da pasta temporária.
 */
static fs::path pasta() {
    return fs::temp_directory_path() / "bovicount_test_contagem";
}

/** @brief Apaga e recria a pasta temporária, isolando um teste do outro. */
static void limpar() {
    std::error_code erro;
    fs::remove_all(pasta(), erro);
    fs::create_directories(pasta());
}

/**
 * @brief Monta o caminho completo de um arquivo dentro da pasta temporária.
 *
 * @param nome Nome do arquivo.
 * @return Caminho completo do arquivo.
 */
static std::string caminho(const std::string& nome) {
    return (pasta() / nome).string();
}

/**
 * @brief Grava bytes crus em um arquivo.
 *
 * @param arquivo Caminho do arquivo a criar.
 * @param bytes   Bytes a gravar.
 */
static void escrever(const std::string& arquivo, const std::vector<unsigned char>& bytes) {
    std::ofstream saida(arquivo, std::ios::binary);
    saida.write(reinterpret_cast<const char*>(bytes.data()),
                static_cast<std::streamsize>(bytes.size()));
}

/**
 * @brief Acrescenta um inteiro de 32 bits em big-endian (byte mais significativo primeiro),
 *        como o PNG grava as dimensões.
 *
 * @param v Bytes do arquivo em construção.
 * @param n Valor a acrescentar.
 */
static void escreverBE32(std::vector<unsigned char>& v, std::uint32_t n) {
    v.push_back(static_cast<unsigned char>(n >> 24));
    v.push_back(static_cast<unsigned char>(n >> 16));
    v.push_back(static_cast<unsigned char>(n >> 8));
    v.push_back(static_cast<unsigned char>(n));
}

/**
 * @brief Acrescenta um inteiro de 16 bits em big-endian, como o JPEG grava as dimensões.
 *
 * @param v Bytes do arquivo em construção.
 * @param n Valor a acrescentar.
 */
static void escreverBE16(std::vector<unsigned char>& v, std::uint16_t n) {
    v.push_back(static_cast<unsigned char>(n >> 8));
    v.push_back(static_cast<unsigned char>(n));
}

/**
 * @brief Cria um arquivo só com cabeçalho PNG válido (assinatura + IHDR) nas dimensões dadas.
 *
 * @param nome    Nome do arquivo.
 * @param largura Largura em pixels.
 * @param altura  Altura em pixels.
 * @return Caminho completo do arquivo criado.
 */
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

/**
 * @brief Cria um arquivo só com cabeçalho JPEG válido (SOI + SOF0) nas dimensões dadas.
 *
 * @param nome    Nome do arquivo.
 * @param largura Largura em pixels.
 * @param altura  Altura em pixels.
 * @return Caminho completo do arquivo criado.
 */
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

/**
 * @brief Cria um arquivo com o texto dado, para testar formato errado ou cabeçalho inválido.
 *
 * @param nome  Nome do arquivo.
 * @param texto Conteúdo do arquivo.
 * @return Caminho completo do arquivo criado.
 */
static std::string criarTexto(const std::string& nome, const std::string& texto) {
    const std::string arquivo = caminho(nome);
    std::ofstream saida(arquivo, std::ios::binary);
    saida << texto;
    return arquivo;
}

/**
 * @brief Motor de IA simulado: devolve sempre o mesmo número de animais, sem analisar a imagem.
 *
 * Pode ser configurado para falhar e conta quantas vezes foi chamado; assim os testes verificam
 * se o serviço recusou o pedido antes de chamar o motor.
 */
class MotorSimulado : public MotorVisao {
private:
    /** @brief Quantos animais o motor simula detectar. */
    int animais;
    /** @brief Se @c true, o motor termina indicando falha. */
    bool falha;

public:
    /** @brief Quantas vezes detect() foi chamado. */
    int chamadas;

    /**
     * @brief Cria o motor simulado.
     *
     * @param animais Quantos animais detectar em cada chamada.
     * @param falha   @c true para simular uma falha do motor.
     */
    MotorSimulado(int animais, bool falha = false)
        : animais(animais), falha(falha), chamadas(0) {}

    /**
     * @brief Marca a quantidade configurada de caixas na contagem de saída.
     *
     * Mesmo quando simula falha, escreve na saída, para provar que o serviço não deixa esse
     * resultado vazar.
     *
     * @param imagem   Imagem recebida (não é analisada).
     * @param contagem Contagem de saída.
     * @return @c false se o motor foi configurado para falhar.
     */
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

/** @brief Sessão de um Gestor (id 1, fazenda 1). */
static Sessao gestor()        { return Sessao(Usuario(1, "Gestor", 1)); }
/** @brief Sessão de um Funcionário (id 5, fazenda 1). */
static Sessao funcionario()   { return Sessao(Usuario(5, "Funcionário", 1)); }
/** @brief Sessão de um Administrador (id 2, fazenda 1). */
static Sessao administrador() { return Sessao(Usuario(2, "Administrador", 1)); }

/**
 * @brief Contagem de apoio, com 3 caixas, para checar que uma recusa não altera a saída.
 *
 * @return Contagem da foto anterior.jpg com 3 animais e 9 segundos de análise.
 */
static Contagem contagemAnterior() {
    Colecao<Retangulo> caixas;
    caixas.add(Retangulo(1, 1, 5, 5));
    caixas.add(Retangulo(2, 2, 5, 5));
    caixas.add(Retangulo(3, 3, 5, 5));
    return Contagem(Imagem("anterior.jpg"), caixas, 9.0);
}

/**
 * @brief Confere que a contagem ainda é a de contagemAnterior().
 *
 * @param c Contagem a verificar.
 * @return @c true se total, foto e tempo de análise não mudaram.
 */
static bool inalterada(const Contagem& c) {
    return c.total() == 3 && c.image().path() == "anterior.jpg"
        && c.processingSeconds() == 9.0;
}

// ---------- Contar ----------

/**
 * @brief Cenário 1 de 4.2: o Funcionário envia a foto e o sistema marca 185 animais.
 *
 * Confere o sucesso, o total e o número de caixas iguais a 185, a foto da contagem, o tempo de
 * análise de até 30 segundos e uma única chamada ao motor.
 */
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

/** @brief Gestor e Administrador também podem contar: ambos obtêm 40 animais. */
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

/**
 * @brief Formatos aceitos: jpeg, jpg e png, sem diferenciar maiúsculas.
 *
 * As cinco fotos são aceitas e o motor é chamado cinco vezes.
 */
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

/**
 * @brief Cenário 3 de 4.2: pasto sem animais mostra total 0, um resultado válido, sem nenhuma
 *        caixa.
 */
static void testeContagemZero() {
    limpar();
    MotorSimulado motor(0);
    ServicoContagem servico(funcionario(), motor);

    Contagem c;
    assert(servico.countByImage(Imagem(criarPNG("vazio.png", 640, 480)), c));
    assert(c.total() == 0);
    assert(c.boxes().size() == 0);
}

/**
 * @brief Uma nova contagem substitui o resultado anterior: a contagem de saída passa a ter 7
 *        animais.
 */
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