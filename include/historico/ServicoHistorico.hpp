/**
 * @file ServicoHistorico.hpp
 * @brief Declaração da classe ServicoHistorico.
 */
#ifndef SERVICOHISTORICO_HPP
#define SERVICOHISTORICO_HPP

#include <vector>
#include "Colecao.hpp"
#include "Consulta.hpp"
#include "Contagem.hpp"
#include "FiltroHistorico.hpp"
#include "Registro.hpp"
#include "RepositorioHistorico.hpp"
#include "Sessao.hpp"

/**
 * @class ServicoHistorico
 * @brief Serviço responsável pelo histórico de contagens.
 *
 * Salva as contagens confirmadas pelo usuário, permite consultá-las e mede o
 * erro da contagem automática. Também é uma Consulta: as contagens salvas são
 * buscadas com query(filtro), como as demais buscas do sistema.
 *
 * O serviço é criado com a Sessao do usuário que entrou no sistema; por isso as
 * operações não perguntam quem está pedindo, o próprio serviço consulta a Sessao
 * e libera só o que o papel permite:
 *
 * | Operação    | Gestor | Administrador | Funcionário |
 * |-------------|:------:|:-------------:|:-----------:|
 * | save        |  sim   |      sim      |     sim     |
 * | query       |  sim   |      sim      |     não     |
 * | errorRate   |  sim   |      sim      |     não     |
 *
 * Qualquer outro papel é recusado em todas as operações. Cada usuário enxerga
 * apenas os dados da própria fazenda.
 *
 * @par Exemplo
 * @code
 * Usuario joao(5, "Funcionário", 1);
 * ServicoHistorico historico(Sessao(joao));
 *
 * Registro r;
 * bool salvou = historico.save(contagem, idPasto, 185, r);   // true
 *
 * ServicoHistorico gestor(Sessao(Usuario(1, "Gestor", 1)));
 * FiltroHistorico outubro(Data(2026, 10, 1), Data(2026, 10, 31));
 * Colecao<Registro> lista = gestor.query(outubro);
 * double erro;
 * bool medido = gestor.errorRate(outubro, erro);             // erro = diferença média em %
 * @endcode
 *
 * @see Consulta
 * @see Sessao
 * @see FiltroHistorico
 * @see Registro
 */
class ServicoHistorico : public Consulta<Registro, FiltroHistorico> {
public:
    /**
     * @brief Cria o serviço para o usuário da sessão.
     * @param sessao Quem está usando o sistema. O serviço guarda uma cópia.
     */
    explicit ServicoHistorico(const Sessao& sessao);

    /**
     * @brief Salva uma contagem confirmada no histórico.
     *
     * O sistema, nesta ordem:
     *  -# confere se o papel do usuário pode salvar;
     *  -# confere se a contagem veio de uma foto analisada (caminho da imagem
     *     preenchido) e se o total confirmado não é negativo;
     *  -# confere se o pasto existe na fazenda do usuário;
     *  -# guarda o registro de uma só vez e marca o pasto como "com contagens"
     *     (o que impede a remoção do pasto, ver ServicoPastos::remove). Quem
     *     contou e a data e hora são preenchidos pelo sistema;
     *  -# se algo falhar no meio, desfaz o que já foi feito: nada fica salvo
     *     pela metade.
     *
     * Uma contagem com total 0 (foto sem animais) é válida e pode ser salva.
     *
     * @param contagem        Resultado da análise da foto.
     * @param idPasto         Pasto em que a contagem foi feita.
     * @param totalConfirmado Total que o usuário confirmou (ou corrigiu); não pode ser negativo.
     * @param registro        Recebe a contagem salva, com número, data e hora.
     *                        Só é alterado quando o retorno é true.
     * @return true se salvou; false se recusou (papel sem permissão, contagem
     *         sem foto, total negativo, pasto inexistente ou de outra fazenda).
     */
    bool save(const Contagem& contagem, int idPasto,
              int totalConfirmado, Registro& registro);

    /**
     * @brief Consulta as contagens salvas.
     *
     * Só o Gestor (ou Administrador) consulta. Mostra apenas as contagens da
     * fazenda do usuário que atendem ao filtro: dia dentro do período (primeiro e
     * último dia incluídos) e, quando informados, o pasto e o funcionário.
     *
     * @param filtro Período e, se quiser, pasto e funcionário.
     * @return As contagens encontradas, na ordem em que foram salvas. A lista
     *         vem vazia se não houver contagens ou se o usuário não tiver permissão.
     */
    Colecao<Registro> query(const FiltroHistorico& filtro);

    /**
     * @brief Calcula o erro da contagem automática para o mesmo filtro da consulta.
     *
     * Em cada contagem, a diferença (em valor absoluto) entre o total calculado
     * e o total confirmado é dividida pelo total confirmado. O resultado é a
     * média dessas porcentagens. Contagens com total confirmado 0 ficam fora do
     * cálculo (não há base para a porcentagem), mas continuam aparecendo em query().
     *
     * Exemplo: 185 calculado e 185 confirmado = 0%; 180 e 200 = 10%; média = 5%.
     *
     * @param filtro Período e, se quiser, pasto e funcionário.
     * @param taxa   Recebe a diferença média, em %. Só é alterado quando o retorno é true.
     * @return true se calculou; false se o usuário não tem permissão ou se não há
     *         nenhuma contagem utilizável no filtro.
     */
    bool errorRate(const FiltroHistorico& filtro, double& taxa);

private:
    /**
     * @brief Contagens da fazenda do usuário que atendem ao filtro.
     *
     * Não confere permissão; quem chama é responsável por isso.
     *
     * @param filtro Período, pasto e funcionário desejados.
     * @return Os registros que atendem ao filtro.
     */
    std::vector<Registro> filtrar(const FiltroHistorico& filtro) const;

    Sessao sessao;                       ///< Quem está usando o sistema (cópia).
    RepositorioHistorico& repositorio;   ///< Onde as contagens ficam guardadas.
};

#endif