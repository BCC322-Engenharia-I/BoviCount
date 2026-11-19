/**
 * @file Data.hpp
 * @brief Declaração da classe Data.
 */
#ifndef DATA_HPP
#define DATA_HPP

/**
 * @class Data
 * @brief Uma data do calendário (ano, mês e dia), por exemplo 03/10/2026.
 *
 * Guarda apenas a data, sem hora. É um tipo-valor: pode ser copiado e
 * atribuído livremente. Nesta versão a classe não valida os valores (não
 * confere, por exemplo, se o mês está entre 1 e 12); quem cria a data é
 * responsável por informar valores coerentes.
 *
 * Usada pelo ServicoHistorico para comparar o dia de um Registro com o
 * período de um FiltroHistorico.
 *
 * @see DataHora
 * @see FiltroHistorico
 */
class Data {
public:
    /**
     * @brief Cria a data zerada (0/0/0).
     *
     * Existe só para permitir objetos ainda não preenchidos, como um
     * Registro padrão. Não representa uma data válida.
     */
    Data();

    /**
     * @brief Cria uma data.
     * @param ano Ano com quatro dígitos (ex.: 2026).
     * @param mes Mês, de 1 a 12.
     * @param dia Dia do mês, de 1 a 31.
     */
    Data(int ano, int mes, int dia);

    /** @brief Devolve o ano. */
    int year() const;

    /** @brief Devolve o mês (1 a 12). */
    int month() const;

    /** @brief Devolve o dia do mês. */
    int day() const;

    /**
     * @brief Compara duas datas por igualdade.
     * @param outra Data a comparar.
     * @return true se ano, mês e dia são iguais.
     */
    bool operator==(const Data& outra) const;

    /**
     * @brief Diz se esta data vem antes de outra.
     *
     * Compara primeiro o ano, depois o mês e por fim o dia.
     *
     * @param outra Data a comparar.
     * @return true se esta data é anterior a @p outra; false se for igual ou posterior.
     */
    bool operator<(const Data& outra) const;

private:
    int ano; ///< Ano.
    int mes; ///< Mês (1 a 12).
    int dia; ///< Dia do mês.
};

#endif