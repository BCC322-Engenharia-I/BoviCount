/**
 * @file FiltroHistorico.hpp
 * @brief Declaração da classe FiltroHistorico.
 */

#ifndef FILTROHISTORICO_HPP
#define FILTROHISTORICO_HPP

#include "core/Data.hpp"

/**
 * @class FiltroHistorico
 * @brief Critério para buscar contagens salvas: período e, se quiser, pasto e funcionário.
 *
 * O período é obrigatório e **inclui** o primeiro e o último dia. O pasto e o
 * funcionário são opcionais: o valor 0 significa "todos".
 *
 * É o tipo de filtro da Consulta implementada pelo ServicoHistorico.
 *
 * @see ServicoHistorico::query
 * @see ServicoHistorico::errorRate
 */
class FiltroHistorico {
public:
    /**
     * @brief Cria um filtro.
     * @param inicio     Primeiro dia do período (incluído).
     * @param fim        Último dia do período (incluído).
     * @param idPasto    Número do pasto; 0 = todos os pastos.
     * @param idOperador Número do funcionário; 0 = todos os funcionários.
     */
    FiltroHistorico(const Data& inicio, const Data& fim,
                    int idPasto = 0,
                    int idOperador = 0);

    /** @brief Devolve o primeiro dia do período. */
    Data start() const;

    /** @brief Devolve o último dia do período. */
    Data end() const;

    /** @brief Devolve o número do pasto buscado (0 = todos). */
    int pastureId() const;

    /** @brief Devolve o número do funcionário buscado (0 = todos). */
    int operatorId() const;

private:
    Data inicio;    ///< Primeiro dia do período.
    Data fim;       ///< Último dia do período.
    int idPasto;    ///< Pasto buscado (0 = todos).
    int idOperador; ///< Funcionário buscado (0 = todos).
};

#endif