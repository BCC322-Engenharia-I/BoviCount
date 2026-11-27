/**
 * @file DataHora.hpp
 * @brief Declaração da classe DataHora.
 */

#ifndef DATAHORA_HPP
#define DATAHORA_HPP

#include "core/Data.hpp"

/**
 * @class DataHora
 * @brief Uma data com hora; registra o momento exato em que uma contagem foi salva.
 *
 * Tipo-valor composto por uma Data e por hora, minuto e segundo. Assim como
 * Data, não valida os valores informados.
 *
 * @see Data
 * @see Registro::dateTime
 */
class TesteDataHora;

class DataHora {
public:
    /** @brief Cria uma data e hora zeradas (não representa um momento válido). */
    DataHora();

    /**
     * @brief Cria uma data com hora.
     * @param data    Dia do momento.
     * @param hora    Hora, de 0 a 23.
     * @param minuto  Minuto, de 0 a 59.
     * @param segundo Segundo, de 0 a 59.
     */
    DataHora(const Data& data, int hora, int minuto, int segundo);

    /** @brief Construtor de cópia: copia o dia e a hora de outra DataHora. */
    DataHora(const DataHora&) = default;

    /** @brief Atribuição por cópia: copia o dia e a hora de outra DataHora. */
    DataHora& operator=(const DataHora&) = default;

    /** @brief Destrutor. */
    ~DataHora() = default;

    /**
     * @brief Devolve o momento atual, segundo o relógio do computador.
     *
     * É usada pelo ServicoHistorico::save para carimbar a contagem salva.
     *
     * @return O instante atual em horário local; se o relógio não puder ser
     *         lido, devolve uma DataHora zerada.
     */
    static DataHora agora();

    /** @brief Devolve o dia (sem a hora). */
    Data date() const;

    /** @brief Devolve a hora (0 a 23). */
    int hour() const;

    /** @brief Devolve o minuto (0 a 59). */
    int minute() const;

    /** @brief Devolve o segundo (0 a 59). */
    int second() const;

private:
    friend class TesteDataHora;
    Data data;   ///< Dia do momento.
    int hora;    ///< Hora (0 a 23).
    int minuto;  ///< Minuto (0 a 59).
    int segundo; ///< Segundo (0 a 59).
};

#endif