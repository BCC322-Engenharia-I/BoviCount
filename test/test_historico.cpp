#include <cassert>
#include <cmath>
#include <iostream>
#include "usuario/Usuario.hpp"
#include "usuario/Sessao.hpp"
#include "pasto/ServicoPastos.hpp"
#include "historico/ServicoHistorico.hpp"

static void limpar() {
    RepositorioPastos::instancia().limpar();
    RepositorioHistorico::instancia().limpar();
}

static bool quase(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

// Monta o resultado da analise de uma foto com 'animais' marcacoes.
static Contagem criarContagem(const std::string& caminho, int animais) {
    Colecao<Retangulo> caixas;
    for (int i = 0; i < animais; ++i) {
        caixas.add(Retangulo(i, i, 10, 10));
    }
    return Contagem(Imagem(caminho), caixas, 1.5);
}

// Grava direto no repositorio uma contagem com data conhecida (o servico sempre
// usa a data de hoje; aqui precisamos de datas fixas para testar os filtros).
static void semear(int fazenda, const Data& data, int operador, int pasto,
                   int calculado, int confirmado) {
    RepositorioHistorico::instancia().inserir(DataHora(data, 9, 30, 0), operador, pasto,
                                              calculado, confirmado, "marcada.jpg", fazenda);
}

static FiltroHistorico outubro() {
    return FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31));
}

// ---------- Data e DataHora ----------

static void testeData() {
    assert(Data(2026, 10, 1) < Data(2026, 10, 2));
    assert(Data(2026, 9, 30) < Data(2026, 10, 1));
    assert(Data(2025, 12, 31) < Data(2026, 1, 1));
    assert(!(Data(2026, 10, 1) < Data(2026, 10, 1)));
    assert(Data(2026, 10, 1) == Data(2026, 10, 1));
    assert(!(Data(2026, 10, 1) == Data(2026, 10, 2)));

    DataHora dh(Data(2026, 10, 6), 14, 30, 5);
    assert(dh.date() == Data(2026, 10, 6));
    assert(dh.hour() == 14 && dh.minute() == 30 && dh.second() == 5);

    DataHora agora = DataHora::agora();
    assert(agora.date().year() >= 2026);
}

// ---------- Salvar ----------

// Cenario 1: Funcionario Joao conta 185 e salva com 185 confirmado.
static void testeSalvarFuncionario() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    ServicoHistorico historicoGestor(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));

    Data antes = DataHora::agora().date();
    Registro r;
    assert(joao.save(criarContagem("vistoria_20261006_pasto1.jpg", 185), fundo.id(), 185, r));
    Data depois = DataHora::agora().date();

    assert(r.id() != 0);
    assert(r.operatorId() == 5);
    assert(r.pastureId() == fundo.id());
    assert(r.automaticTotal() == 185);
    assert(r.confirmedTotal() == 185);
    assert(r.imagePath() == "vistoria_20261006_pasto1.jpg");
    assert(!(r.dateTime().date() < antes) && !(depois < r.dateTime().date()));

    // o Gestor enxerga o que o Funcionario salvou
    Colecao<Registro> lista = historicoGestor.query(FiltroHistorico(Data(2000, 1, 1), Data(2999, 12, 31)));
    assert(lista.size() == 1);
    Iterador<Registro> it = lista.iterator();
    assert(it.hasNext());
    const Registro& salvo = it.next();
    assert(salvo.id() == r.id());
    assert(salvo.operatorId() == 5);
    assert(salvo.confirmedTotal() == 185);
}

// Cenario 2: o sistema contou 180, o usuario corrige para 200.
static void testeSalvarComCorrecao() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto fundo;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));

    Registro r;
    assert(joao.save(criarContagem("foto.jpg", 180), fundo.id(), 200, r));
    assert(r.automaticTotal() == 180);
    assert(r.confirmedTotal() == 200);
}

static void testeSalvarGestorEAdministrador() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico comoGestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico comoAdmin(Sessao(Usuario(2, "Administrador", 1)));

    Pasto fundo;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));

    Registro a, b;
    assert(comoGestor.save(criarContagem("a.png", 10), fundo.id(), 10, a));
    assert(comoAdmin.save(criarContagem("b.jpeg", 20), fundo.id(), 20, b));
    assert(a.id() != b.id());
    assert(a.operatorId() == 1);
    assert(b.operatorId() == 2);
}

// Pasto sem animais: total 0 e um resultado valido.
static void testeSalvarContagemZero() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto vazio;
    assert(gestor.create(DadosPasto("Pasto Vazio", "Sede", 50), vazio));

    Registro r;
    assert(joao.save(criarContagem("vazio.jpg", 0), vazio.id(), 0, r));
    assert(r.automaticTotal() == 0);
    assert(r.confirmedTotal() == 0);
}

// Cenarios 3 e 4 (recusas): nada e salvo.
static void testeSalvarRecusas() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoPastos outraFazenda(Sessao(Usuario(9, "Gestor", 2)));
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    ServicoHistorico historicoGestor(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo, alheio;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));
    assert(outraFazenda.create(DadosPasto("Pasto Alheio", "Longe", 80), alheio));

    // pasto que nao existe
    Registro r1;
    assert(!joao.save(criarContagem("foto.jpg", 185), 999, 185, r1));
    assert(r1.id() == 0);

    // pasto de outra fazenda
    Registro r2;
    assert(!joao.save(criarContagem("foto.jpg", 185), alheio.id(), 185, r2));
    assert(r2.id() == 0);

    // contagem sem foto analisada
    Registro r3;
    assert(!joao.save(Contagem(), fundo.id(), 0, r3));
    assert(r3.id() == 0);
    assert(!joao.save(criarContagem("   ", 10), fundo.id(), 10, r3));

    // total confirmado negativo
    Registro r4;
    assert(!joao.save(criarContagem("foto.jpg", 185), fundo.id(), -1, r4));
    assert(r4.id() == 0);

    // papel desconhecido
    ServicoHistorico intruso(Sessao(Usuario(7, "Visitante", 1)));
    Registro r5;
    assert(!intruso.save(criarContagem("foto.jpg", 185), fundo.id(), 185, r5));
    assert(r5.id() == 0);

    // nada foi salvo
    assert(historicoGestor.query(FiltroHistorico(Data(2000, 1, 1), Data(2999, 12, 31))).size() == 0);
}

// Integracao com os pastos: pasto com contagem salva nao pode ser removido.
static void testeSalvarBloqueiaRemocaoDoPasto() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto fundo, brejo;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));
    assert(gestor.create(DadosPasto("Pasto Brejo", "Rio Sul", 120), brejo));

    Registro r;
    assert(joao.save(criarContagem("foto.jpg", 185), fundo.id(), 185, r));

    assert(!gestor.remove(fundo.id()));
    assert(gestor.query(FiltroPasto("Pasto Fundo")).size() == 1);
    assert(gestor.remove(brejo.id()));
}

// Uma recusa nao marca o pasto como "com contagens".
static void testeRecusaNaoBloqueiaPasto() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto fundo;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));

    Registro r;
    assert(!joao.save(criarContagem("foto.jpg", 10), fundo.id(), -5, r));
    assert(gestor.remove(fundo.id()));
}

// ---------- Consultar ----------

// Cenario 2 (4.4): o Funcionario nao ve nada.
static void testeConsultaFuncionario() {
    limpar();
    semear(1, Data(2026, 10, 6), 5, 1, 185, 185);

    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    assert(joao.query(outubro()).size() == 0);

    double taxa = -1.0;
    assert(!joao.errorRate(outubro(), taxa));
    assert(quase(taxa, -1.0));
}

static void testeConsultaFiltros() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 185, 185);   // primeiro dia do periodo
    semear(1, Data(2026, 10, 15), 6, 2, 180, 200);
    semear(1, Data(2026, 10, 31), 5, 2, 100, 100);  // ultimo dia do periodo
    semear(1, Data(2026, 11, 1), 5, 1, 50, 50);     // fora (depois)
    semear(1, Data(2026, 9, 30), 5, 1, 10, 10);     // fora (antes)
    semear(2, Data(2026, 10, 10), 9, 3, 1, 1);      // outra fazenda

    ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico outra(Sessao(Usuario(9, "Gestor", 2)));

    // periodo (inclui o primeiro e o ultimo dia)
    assert(gestor.query(outubro()).size() == 3);
    assert(gestor.query(FiltroHistorico(Data(2026, 1, 1), Data(2026, 12, 31))).size() == 5);
    assert(gestor.query(FiltroHistorico(Data(2026, 10, 15), Data(2026, 10, 15))).size() == 1);

    // pasto
    Colecao<Registro> doPasto1 = gestor.query(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 1));
    assert(doPasto1.size() == 1);
    assert(doPasto1.iterator().next().pastureId() == 1);

    // funcionario
    assert(gestor.query(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 0, 5)).size() == 2);
    assert(gestor.query(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 0, 6)).size() == 1);

    // pasto + funcionario
    assert(gestor.query(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 2, 5)).size() == 1);
    assert(gestor.query(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 1, 6)).size() == 0);

    // periodo sem contagens = lista vazia
    assert(gestor.query(FiltroHistorico(Data(2025, 1, 1), Data(2025, 12, 31))).size() == 0);

    // cada fazenda ve so o que e seu
    assert(outra.query(outubro()).size() == 1);
    assert(outra.query(outubro()).iterator().next().operatorId() == 9);
}

static void testeConsultaAdministrador() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 185, 185);
    semear(1, Data(2026, 10, 15), 6, 2, 180, 200);

    ServicoHistorico admin(Sessao(Usuario(2, "Administrador", 1)));
    assert(admin.query(outubro()).size() == 2);

    double taxa = 0.0;
    assert(admin.errorRate(outubro(), taxa));
    assert(quase(taxa, 5.0));
}

static void testeConsultaPapelDesconhecido() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 185, 185);

    ServicoHistorico intruso(Sessao(Usuario(7, "Visitante", 1)));
    assert(intruso.query(outubro()).size() == 0);
    double taxa = -1.0;
    assert(!intruso.errorRate(outubro(), taxa));
}

// ---------- Erro da contagem ----------

// Cenario 1 (4.4): 185/185 e 180/200 => 5%.
static void testeErroCenarioPrincipal() {
    limpar();
    semear(1, Data(2026, 10, 6), 5, 1, 185, 185);
    semear(1, Data(2026, 10, 20), 5, 1, 180, 200);

    ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));
    assert(gestor.query(outubro()).size() == 2);

    double taxa = -1.0;
    assert(gestor.errorRate(outubro(), taxa));
    assert(quase(taxa, 5.0));
}

// Cenario 3 (4.4): sem contagens no periodo, nao calcula o erro.
static void testeErroSemContagens() {
    limpar();
    ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));

    double taxa = -1.0;
    assert(!gestor.errorRate(outubro(), taxa));
    assert(quase(taxa, -1.0));  // 'taxa' nao foi alterada

    // contagens existem, mas em outro periodo
    semear(1, Data(2026, 9, 1), 5, 1, 10, 10);
    assert(!gestor.errorRate(outubro(), taxa));
    assert(quase(taxa, -1.0));
}

// A diferenca conta nos dois sentidos (calculou a mais ou a menos).
static void testeErroDiferencaAbsoluta() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 220, 200);   // 10% a mais
    semear(1, Data(2026, 10, 2), 5, 1, 180, 200);   // 10% a menos

    ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));
    double taxa = 0.0;
    assert(gestor.errorRate(outubro(), taxa));
    assert(quase(taxa, 10.0));
}

// Contagens com total confirmado 0 ficam fora do calculo.
static void testeErroIgnoraConfirmadoZero() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 3, 0);       // fora do calculo
    semear(1, Data(2026, 10, 2), 5, 1, 180, 200);   // 10%

    ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));
    double taxa = 0.0;
    assert(gestor.errorRate(outubro(), taxa));
    assert(quase(taxa, 10.0));
    assert(gestor.query(outubro()).size() == 2);   // a lista continua mostrando as duas

    // so ha contagens com confirmado 0: nao ha o que medir
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 0, 0);
    double taxa2 = -1.0;
    assert(!gestor.errorRate(outubro(), taxa2));
    assert(quase(taxa2, -1.0));
}

// O erro usa o mesmo filtro da consulta e so a fazenda do usuario.
static void testeErroRespeitaFiltroEFazenda() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 185, 185);   // 0%
    semear(1, Data(2026, 10, 2), 6, 2, 180, 200);   // 10%
    semear(2, Data(2026, 10, 3), 9, 3, 50, 100);    // 50%, outra fazenda

    ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));
    double taxa = 0.0;

    assert(gestor.errorRate(outubro(), taxa));
    assert(quase(taxa, 5.0));

    assert(gestor.errorRate(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 2), taxa));
    assert(quase(taxa, 10.0));

    assert(gestor.errorRate(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 0, 5), taxa));
    assert(quase(taxa, 0.0));

    assert(gestor.errorRate(FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31), 0, 9), taxa) == false);
}

// Fluxo completo: Gestor cadastra, Funcionario salva, Gestor consulta o erro.
static void testeFluxoCompleto() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    ServicoHistorico historico(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));

    Registro a, b;
    assert(joao.save(criarContagem("a.jpg", 185), fundo.id(), 185, a));
    assert(joao.save(criarContagem("b.jpg", 180), fundo.id(), 200, b));

    FiltroHistorico tudo(Data(2000, 1, 1), Data(2999, 12, 31));
    assert(historico.query(tudo).size() == 2);

    double taxa = 0.0;
    assert(historico.errorRate(tudo, taxa));
    assert(quase(taxa, 5.0));

    assert(!gestor.remove(fundo.id()));
}

int main() {
    std::cout << "===== TESTE DE HISTORICO =====" << std::endl;
    testeData();
    testeSalvarFuncionario();
    testeSalvarComCorrecao();
    testeSalvarGestorEAdministrador();
    testeSalvarContagemZero();
    testeSalvarRecusas();
    testeSalvarBloqueiaRemocaoDoPasto();
    testeRecusaNaoBloqueiaPasto();
    testeConsultaFuncionario();
    testeConsultaFiltros();
    testeConsultaAdministrador();
    testeConsultaPapelDesconhecido();
    testeErroCenarioPrincipal();
    testeErroSemContagens();
    testeErroDiferencaAbsoluta();
    testeErroIgnoraConfirmadoZero();
    testeErroRespeitaFiltroEFazenda();
    testeFluxoCompleto();
    std::cout << "  test_historico: OK" << std::endl;
    std::cout << "==============================" << std::endl;
    return 0;
}
