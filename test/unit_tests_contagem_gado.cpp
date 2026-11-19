#include "unit_tests_contagem_gado.h"
#include "contagem_gado.h"
#include "servico_contagem_gado.h"
#include "usuario_contagem_gado.h"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>

using namespace contagem_gado;

namespace {

// Substitui somente a dependencia de inteligencia artificial.
// Assim podemos testar as classes do BoviCount sem executar uma IA real.
class MotorFalso : public MotorVisao {
private:
    int quantidade_;
    bool falha_;
    int chamadas_ = 0;

public:
    MotorFalso(int quantidade, bool falha = false)
        : quantidade_(quantidade), falha_(falha) {}

    bool detect(const Imagem& imagem, Contagem& contagem) override {
        ++chamadas_;
        Colecao<Retangulo> marcacoes;
        for (int i = 0; i < quantidade_; ++i) {
            marcacoes.add(Retangulo(i * 10, 0, 10, 10));
        }
        contagem = Contagem(imagem, marcacoes, 0.01);
        return !falha_;
    }

    int chamadas() const { return chamadas_; }
};

std::string arquivoTeste(const std::string& nome) {
    return (std::filesystem::path(__FILE__).parent_path()
            / "fixtures" / nome).string();
}

void aprovado(const char* nome) {
    std::cout << "[OK] " << nome << '\n';
}

} // namespace

// Testa apenas o construtor e o metodo path() da classe Imagem.
void testeImagem() {
    Imagem vazia;
    Imagem foto("gado.jpg");
    assert(vazia.path() == "");
    assert(foto.path() == "gado.jpg");
    aprovado("Imagem");
}

// Testa o armazenamento de coordenadas e dimensoes de um Retangulo.
void testeRetangulo() {
    Retangulo r(10, 20, 30, 40);
    assert(r.x() == 10);
    assert(r.y() == 20);
    assert(r.largura() == 30);
    assert(r.altura() == 40);
    aprovado("Retangulo");
}

// Testa os metodos add() e size() de Colecao.
void testeColecao() {
    Colecao<Retangulo> marcacoes;
    assert(marcacoes.size() == 0);
    marcacoes.add(Retangulo(0, 0, 10, 10));
    marcacoes.add(Retangulo(20, 0, 10, 10));
    assert(marcacoes.size() == 2);
    aprovado("Colecao");
}

// Testa o construtor e os metodos total(), boxes() e image().
void testeContagem() {
    Contagem inicial;
    assert(inicial.total() == 0);

    Colecao<Retangulo> marcacoes;
    marcacoes.add(Retangulo(1, 2, 3, 4));
    marcacoes.add(Retangulo(5, 6, 7, 8));
    Contagem resultado(Imagem("pasto.png"), marcacoes, 0.5);

    assert(resultado.total() == 2);
    assert(resultado.boxes().size() == 2);
    assert(resultado.image().path() == "pasto.png");
    aprovado("Contagem");
}

// Testa isoladamente a leitura do tempo guardado na Contagem.
void testeTempoContagem() {
    Colecao<Retangulo> vazia;
    Contagem resultado(Imagem("pasto.png"), vazia, 0.75);
    assert(resultado.processingSeconds() == 0.75);
    aprovado("Tempo da contagem");
}

// Testa os atributos do Usuario usados pelo servico.
void testeUsuario() {
    Usuario usuario(5, "Funcionário", 3);
    assert(usuario.id() == 5);
    assert(usuario.role() == "Funcionário");
    assert(usuario.propertyId() == 3);
    aprovado("Usuario");
}

// Testa o acesso ao usuario atraves da Sessao.
void testeSessao() {
    Usuario usuario(7, "Gestor", 2);
    Sessao sessao(usuario);
    assert(sessao.user().id() == 7);
    assert(sessao.user().role() == "Gestor");
    assert(sessao.user().propertyId() == 2);
    aprovado("Sessao");
}

// Testa countByImage() com sua dependencia MotorVisao substituida.
void testeServicoContagemValida() {
    Usuario usuario(5, "Funcionário", 1);
    Sessao sessao(usuario);
    MotorFalso motor(2);
    ServicoContagem servico(sessao, motor);
    Contagem resultado;

    bool ok = servico.countByImage(Imagem(arquivoTeste("foto_185.jpg")), resultado);
    assert(ok);
    assert(resultado.total() == 2); // Numero escolhido no motor falso, nao na foto.
    assert(motor.chamadas() == 1);
    aprovado("ServicoContagem: motor chamado e resultado recebido");
}

// Testa se total zero continua sendo uma contagem valida.
void testeServicoContagemVazia() {
    Usuario usuario(5, "Funcionário", 1);
    Sessao sessao(usuario);
    MotorFalso motor(0);
    ServicoContagem servico(sessao, motor);
    Contagem resultado;

    bool ok = servico.countByImage(Imagem(arquivoTeste("foto_vazia.png")), resultado);
    assert(ok);
    assert(resultado.total() == 0);
    assert(motor.chamadas() == 1);
    aprovado("ServicoContagem: resultado zero");
}

// Testa a rejeicao de um formato nao permitido antes de chamar o motor.
void testeFormatoInvalido() {
    Usuario usuario(5, "Funcionário", 1);
    Sessao sessao(usuario);
    MotorFalso motor(3);
    ServicoContagem servico(sessao, motor);
    Contagem resultado;

    bool ok = servico.countByImage(Imagem(arquivoTeste("relatorio.pdf")), resultado);
    assert(!ok);
    assert(motor.chamadas() == 0);
    aprovado("ServicoContagem: PDF rejeitado");
}

// Testa a rejeicao quando o caminho nao aponta para um arquivo existente.
void testeArquivoInexistente() {
    Usuario usuario(5, "Funcionário", 1);
    Sessao sessao(usuario);
    MotorFalso motor(3);
    ServicoContagem servico(sessao, motor);
    Contagem resultado;

    const std::string caminho = arquivoTeste("arquivo_que_nao_existe.jpg");
    assert(!std::filesystem::exists(caminho));
    bool ok = servico.countByImage(Imagem(caminho), resultado);
    assert(!ok);
    assert(motor.chamadas() == 0);
    aprovado("ServicoContagem: arquivo inexistente");
}

// Testa a verificacao de permissao feita pelo servico.
void testePermissao() {
    Usuario usuario(9, "Visitante", 1);
    Sessao sessao(usuario);
    MotorFalso motor(3);
    ServicoContagem servico(sessao, motor);
    Contagem resultado;

    bool ok = servico.countByImage(Imagem(arquivoTeste("foto_185.jpg")), resultado);
    assert(!ok);
    assert(motor.chamadas() == 0);
    aprovado("ServicoContagem: usuario sem permissao");
}

// Testa se a contagem anterior e preservada se o motor falhar.
void testeFalhaMotor() {
    Usuario usuario(5, "Funcionário", 1);
    Sessao sessao(usuario);
    MotorFalso motor(3, true);
    ServicoContagem servico(sessao, motor);

    Colecao<Retangulo> marcacoes;
    marcacoes.add(Retangulo(1, 2, 3, 4));
    Contagem anterior(Imagem("anterior.png"), marcacoes, 2.5);

    bool ok = servico.countByImage(Imagem(arquivoTeste("foto_185.jpg")), anterior);
    assert(!ok);
    assert(motor.chamadas() == 1);
    assert(anterior.total() == 1);
    assert(anterior.image().path() == "anterior.png");
    assert(anterior.processingSeconds() == 2.5);
    aprovado("ServicoContagem: falha nao altera resultado anterior");
}

// Testa os dois limites de resolucao da regra adotada: UHD 3840x2160.
void testeLimite4K() {
    Usuario usuario(5, "Funcionário", 1);
    Sessao sessao(usuario);
    MotorFalso motor(1);
    ServicoContagem servico(sessao, motor);
    Contagem resultado;

    bool noLimite = servico.countByImage(Imagem(arquivoTeste("foto_4k.png")), resultado);
    assert(noLimite);
    assert(motor.chamadas() == 1);

    bool acima = servico.countByImage(Imagem(arquivoTeste("foto_acima_4k.png")), resultado);
    assert(!acima);
    assert(motor.chamadas() == 1);
    aprovado("ServicoContagem: aceita 4K e rejeita acima de 4K");
}
