/**
 * @file test_historico.cpp
 * @brief Testes do ServicoHistorico (salvar, consultar e medir o erro da contagem).
 *
 * Os testes só instanciam o ServicoHistorico: os pastos de apoio são semeados direto
 * no RepositorioPastos (nenhum outro serviço é usado).
 *
 * Os testes seguem os cenários de teste das seções 4.3 e 4.4 do projeto da API
 * e usam os mesmos dados dos exemplos (Pasto Fundo, Funcionário João, outubro de 2026).
 * Compile com `make compile-test-historico` e execute com `make run-test-historico`.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include "usuario/Usuario.hpp"
#include "usuario/Sessao.hpp"
#include "pasto/RepositorioPastos.hpp"
#include "historico/ServicoHistorico.hpp"

/**
 * @brief Apaga os dados dos repositórios de pastos e de histórico.
 *
 * Chamada no início de cada teste para que um não interfira no outro.
 */
static void limpar() {
    RepositorioPastos::instancia().limpar();
    RepositorioHistorico::instancia().limpar();
}

/**
 * @brief Grava direto no RepositorioPastos um pasto de apoio para os testes.
 *
 * @param nome        Nome do pasto.
 * @param localizacao Localização do pasto.
 * @param capacidade  Capacidade do pasto.
 * @param fazenda     Fazenda dona do pasto.
 * @return O pasto gravado, já com o seu número.
 */
static Pasto semearPasto(const std::string& nome, const std::string& localizacao,
                         int capacidade, int fazenda) {
    return RepositorioPastos::instancia().inserir(DadosPasto(nome, localizacao, capacidade), fazenda);
}

/**
 * @brief Compara dois números reais com tolerância.
 */
static bool quase(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

/**
 * @brief Monta o resultado da análise de uma foto.
 *
 * @param caminho Caminho da foto analisada.
 * @param animais Quantidade de marcações (animais) da contagem.
 * @return Uma Contagem com a foto, @p animais marcações e 1,5 s de análise.
 */
static Contagem criarContagem(const std::string& caminho, int animais) {
    Colecao<Retangulo> caixas;
    for (int i = 0; i < animais; ++i) {
        caixas.add(Retangulo(i, i, 10, 10));
    }
    return Contagem(Imagem(caminho), caixas, 1.5);
}

/**
 * @brief Grava direto no repositório uma contagem com data conhecida.
 *
 * O servico sempre usa a data de hoje; os testes de período e de erro precisam de datas fixas.
 * @param fazenda    Fazenda dona do registro.
 * @param data       Dia em que a contagem foi salva.
 * @param operador   Funcionário que contou.
 * @param pasto      Pasto contado.
 * @param calculado  Total calculado pelo sistema.
 * @param confirmado Total confirmado pelo usuário.
 */
static void semear(int fazenda, const Data& data, int operador, int pasto,
                   int calculado, int confirmado) {
    RepositorioHistorico::instancia().inserir(DataHora(data, 9, 30, 0), operador, pasto,
                                              calculado, confirmado, "marcada.jpg", fazenda);
}

/**
 * @brief Filtro de outubro de 2026 (todos os pastos e funcionários).
 */
static FiltroHistorico outubro() {
    return FiltroHistorico(Data(2026, 10, 1), Data(2026, 10, 31));
}

// ---------- Data e DataHora ----------

/**
 * @brief Testa a comparação de Data e os acessos de DataHora.
 *
 * Confere operator< e operator== (virada de mês e de ano), os getters de DataHora e DataHora::agora.
 */
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

/**
 * @brief Cenário 1 de 4.3: o Funcionário João salva 185 contados e 185 confirmados.
 *
 * Confere todos os campos do Registro (quem contou, pasto, totais, foto e data de hoje) e que o Gestor enxerga o registro na consulta.
 */
static void testeSalvarFuncionario() {
    limpar();
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    ServicoHistorico historicoGestor(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo;
    fundo = semearPasto("Pasto Fundo", "Divisa Norte", 200, 1);

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

/**
 * @brief Cenário 2 de 4.3: o sistema contou 180 e o usuário corrige para 200.
 *
 * O histórico guarda 180 como calculado e 200 como confirmado.
 */
static void testeSalvarComCorrecao() {
    limpar();
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto fundo;
    fundo = semearPasto("Pasto Fundo", "Divisa Norte", 200, 1);

    Registro r;
    assert(joao.save(criarContagem("foto.jpg", 180), fundo.id(), 200, r));
    assert(r.automaticTotal() == 180);
    assert(r.confirmedTotal() == 200);
}

/**
 * @brief Gestor e Administrador também podem salvar contagens.
 */
static void testeSalvarGestorEAdministrador() {
    limpar();
    ServicoHistorico comoGestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoHistorico comoAdmin(Sessao(Usuario(2, "Administrador", 1)));

    Pasto fundo;
    fundo = semearPasto("Pasto Fundo", "Divisa Norte", 200, 1);

    Registro a, b;
    assert(comoGestor.save(criarContagem("a.png", 10), fundo.id(), 10, a));
    assert(comoAdmin.save(criarContagem("b.jpeg", 20), fundo.id(), 20, b));
    assert(a.id() != b.id());
    assert(a.operatorId() == 1);
    assert(b.operatorId() == 2);
}

/**
 * @brief Foto sem animais: total 0 é um resultado válido e pode ser salvo.
 */
static void testeSalvarContagemZero() {
    limpar();
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto vazio;
    vazio = semearPasto("Pasto Vazio", "Sede", 50, 1);

    Registro r;
    assert(joao.save(criarContagem("vazio.jpg", 0), vazio.id(), 0, r));
    assert(r.automaticTotal() == 0);
    assert(r.confirmedTotal() == 0);
}

/**
 * @brief Cenário 3 de 4.3 e demais recusas do save: nada é salvo.
 *
 * Cobre pasto inexistente, pasto de outra fazenda, contagem sem foto, total confirmado negativo e papel desconhecido. Em todos os casos o Registro de saída continua vazio (id 0) e o histórico continua sem registros.
 */
static void testeSalvarRecusas() {
    limpar();
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    ServicoHistorico historicoGestor(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo, alheio;
    fundo = semearPasto("Pasto Fundo", "Divisa Norte", 200, 1);
    alheio = semearPasto("Pasto Alheio", "Longe", 80, 2);

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

/**
 * @brief Integração com os pastos: salvar marca o pasto como "com contagens".
 *
 * É essa marca que faz o ServicoPastos::remove recusar a remoção. Um pasto sem
 * contagens salvas não é marcado.
 */
static void testeSalvarMarcaPastoComContagens() {
    limpar();
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto fundo, brejo;
    fundo = semearPasto("Pasto Fundo", "Divisa Norte", 200, 1);
    brejo = semearPasto("Pasto Brejo", "Rio Sul", 120, 1);

    Registro r;
    assert(joao.save(criarContagem("foto.jpg", 185), fundo.id(), 185, r));

    assert(RepositorioPastos::instancia().temContagens(fundo.id()));
    assert(!RepositorioPastos::instancia().temContagens(brejo.id()));
}

/**
 * @brief Um salvamento recusado não marca o pasto como "com contagens".
 */
static void testeRecusaNaoMarcaPasto() {
    limpar();
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto fundo;
    fundo = semearPasto("Pasto Fundo", "Divisa Norte", 200, 1);

    Registro r;
    assert(!joao.save(criarContagem("foto.jpg", 10), fundo.id(), -5, r));
    assert(!RepositorioPastos::instancia().temContagens(fundo.id()));
}

// ---------- Consultar ----------

/**
 * @brief Cenário 2 de 4.4: o Funcionário não vê nenhuma informação.
 *
 * query devolve lista vazia e errorRate devolve false sem alterar a taxa.
 */
static void testeConsultaFuncionario() {
    limpar();
    semear(1, Data(2026, 10, 6), 5, 1, 185, 185);

    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    assert(joao.query(outubro()).size() == 0);

    double taxa = -1.0;
    assert(!joao.errorRate(outubro(), taxa));
    assert(quase(taxa, -1.0));
}

/**
 * @brief Filtros de query: período, pasto, funcionário e fazenda.
 *
 * Confere que o primeiro e o último dia do período são incluídos, que dias fora do período e registros de outra fazenda não aparecem e que um período sem contagens devolve lista vazia.
 */
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

/**
 * @brief O Administrador consulta o histórico e o erro como o Gestor.
 */
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

/**
 * @brief Um papel desconhecido não consulta o histórico nem o erro.
 */
static void testeConsultaPapelDesconhecido() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 185, 185);

    ServicoHistorico intruso(Sessao(Usuario(7, "Visitante", 1)));
    assert(intruso.query(outubro()).size() == 0);
    double taxa = -1.0;
    assert(!intruso.errorRate(outubro(), taxa));
}

// ---------- Erro da contagem ----------

/**
 * @brief Cenário 1 de 4.4: 185/185 e 180/200 resultam em erro de 5%.
 */
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

/**
 * @brief Cenário 3 de 4.4: sem contagens no período, o erro não é calculado.
 *
 * errorRate devolve false e a taxa informada não é alterada.
 */
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

/**
 * @brief O erro conta a diferença nos dois sentidos.
 *
 * Calcular a mais ou a menos pesa igual: 220/200 e 180/200 dão 10% cada.
 */
static void testeErroDiferencaAbsoluta() {
    limpar();
    semear(1, Data(2026, 10, 1), 5, 1, 220, 200);   // 10% a mais
    semear(1, Data(2026, 10, 2), 5, 1, 180, 200);   // 10% a menos

    ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));
    double taxa = 0.0;
    assert(gestor.errorRate(outubro(), taxa));
    assert(quase(taxa, 10.0));
}

/**
 * @brief Contagens com total confirmado 0 ficam fora do cálculo do erro.
 *
 * Elas continuam aparecendo em query. Se só houver contagens assim, errorRate devolve false.
 */
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

/**
 * @brief O erro usa o mesmo filtro da consulta e só a fazenda do usuário.
 */
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

/**
 * @brief Fluxo completo (3.3): com o pasto já cadastrado, o Funcionário salva e o Gestor consulta o erro.
 *
 * Termina conferindo que o pasto ficou marcado como "com contagens".
 */
static void testeFluxoCompleto() {
    limpar();
    ServicoHistorico joao(Sessao(Usuario(5, "Funcionário", 1)));
    ServicoHistorico historico(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo;
    fundo = semearPasto("Pasto Fundo", "Divisa Norte", 200, 1);

    Registro a, b;
    assert(joao.save(criarContagem("a.jpg", 185), fundo.id(), 185, a));
    assert(joao.save(criarContagem("b.jpg", 180), fundo.id(), 200, b));

    FiltroHistorico tudo(Data(2000, 1, 1), Data(2999, 12, 31));
    assert(historico.query(tudo).size() == 2);

    double taxa = 0.0;
    assert(historico.errorRate(tudo, taxa));
    assert(quase(taxa, 5.0));

    assert(RepositorioPastos::instancia().temContagens(fundo.id()));
}

/**
 * @brief Executa todos os testes do histórico.
 *
 * Usa assert: se algum teste falhar, o programa é interrompido.
 *
 * @return 0 quando todos os testes passam.
 */
int main() {
    std::cout << "===== TESTE DE HISTORICO =====" << std::endl;
    testeData();
    testeSalvarFuncionario();
    testeSalvarComCorrecao();
    testeSalvarGestorEAdministrador();
    testeSalvarContagemZero();
    testeSalvarRecusas();
    testeSalvarMarcaPastoComContagens();
    testeRecusaNaoMarcaPasto();
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