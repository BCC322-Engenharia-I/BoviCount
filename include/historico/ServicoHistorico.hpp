#ifndef SERVICOHISTORICO_HPP
#define SERVICOHISTORICO_HPP

#include <vector>
#include "core/Colecao.hpp"
#include "core/Consulta.hpp"
#include "contagem/Contagem.hpp"
#include "historico/FiltroHistorico.hpp"
#include "historico/Registro.hpp"
#include "historico/RepositorioHistorico.hpp"
#include "usuario/Sessao.hpp"

// Servico responsavel pelo historico de contagens. Tambem e uma "Consulta":
// busca contagens salvas com query(filtro).
class ServicoHistorico : public Consulta<Registro, FiltroHistorico> {
public:
    explicit ServicoHistorico(const Sessao& sessao);

    // SALVAR uma contagem. Devolve true se salvou, false se recusou.
    // contagem = resultado da analise da foto
    // idPasto = pasto em que a contagem foi feita
    // totalConfirmado = total que o usuario confirmou (ou corrigiu)
    // registro = recebe a contagem salva, com numero, data e hora
    // O sistema: 1) confere se a contagem veio de uma foto analisada e se o total
    //               nao e negativo;
    //            2) confere se o pasto existe na fazenda do usuario;
    //            3) guarda de uma so vez a foto com as marcacoes e os dados da contagem
    //               (quem contou e a data e hora sao preenchidos pelo sistema);
    //            4) se algo falhar no meio, nada fica salvo pela metade.
    bool save(const Contagem& contagem, int idPasto,
              int totalConfirmado, Registro& registro);

    // CONSULTAR as contagens salvas. So o Gestor (ou Administrador) consulta.
    // Mostra apenas as contagens da fazenda do usuario, dentro do periodo, pasto e
    // funcionario escolhidos. A lista vem vazia se nao houver contagens ou se o
    // usuario nao tiver permissao.
    Colecao<Registro> query(const FiltroHistorico& filtro);

    // CALCULAR O ERRO da contagem automatica no mesmo filtro. Devolve true e preenche
    // 'taxa' com a diferenca media em %. Devolve false se o usuario nao tem permissao
    // ou se nao ha contagens no periodo. Contagens com total confirmado 0 ficam de fora.
    // Exemplo: 185 calculado e 185 confirmado = 0%; 180 e 200 = 10%; media = 5%.
    bool errorRate(const FiltroHistorico& filtro, double& taxa);

private:
    // Contagens da fazenda do usuario que atendem ao filtro (sem checar permissao).
    std::vector<Registro> filtrar(const FiltroHistorico& filtro) const;

    Sessao sessao;
    RepositorioHistorico& repositorio;
};

#endif
