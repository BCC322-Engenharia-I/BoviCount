/**
 * @file Contagem.hpp
 * @brief Declaração da classe Contagem.
 */
#ifndef CONTAGEM_HPP
#define CONTAGEM_HPP

#include "Colecao.hpp"
#include "Imagem.hpp"
#include "Retangulo.hpp"

/**
 * @class Contagem
 * @brief O resultado da análise de uma foto.
 *
 * Traz a foto analisada, uma marcação (Retangulo) para cada animal e o tempo
 * gasto na análise. Ainda **não** é o histórico: só vira histórico quando o
 * usuário confirma o total e chama ServicoHistorico::save.
 *
 * O total de animais é a quantidade de marcações; uma foto sem animais gera
 * uma Contagem válida com total 0.
 *
 * @see Registro
 * @see ServicoHistorico::save
 */
class Contagem {
public:
    /** @brief Cria uma contagem vazia: sem foto, sem marcações, tempo 0. */
    Contagem();

    /**
     * @brief Cria uma contagem a partir do resultado de uma análise.
     * @param imagem   Foto que foi analisada.
     * @param caixas   Uma marcação para cada animal encontrado.
     * @param segundos Tempo gasto na análise, em segundos.
     */
    Contagem(const Imagem& imagem, const Colecao<Retangulo>& caixas, double segundos);

    /** @brief Devolve o total de animais encontrados (número de marcações). */
    int total() const;

    /** @brief Devolve o tempo gasto na análise, em segundos. */
    double processingSeconds() const;

    /** @brief Devolve as marcações, uma para cada animal. */
    Colecao<Retangulo> boxes() const;

    /** @brief Devolve a foto que foi analisada. */
    Imagem image() const;

private:
    Imagem foto;                    ///< Foto analisada.
    Colecao<Retangulo> marcacoes;   ///< Uma marcação por animal.
    double segundos;                ///< Tempo da análise, em segundos.
};

#endif