/**
 * @file Pasto.hpp
 * @brief Declaração da classe Pasto.
 */
#ifndef PASTO_H
#define PASTO_H

#include <string>
#include "pasto/DadosPasto.hpp"

/**
 * @brief Um pasto já cadastrado.
 *
 * Reúne os dados informados (DadosPasto), o número que identifica o pasto
 * e a fazenda a que ele pertence. Um Pasto construído por padrão tem
 * id 0, que indica "ainda não cadastrado".
 *
 * @see RepositorioPastos
 */
class Pasto {
private:
    int idPasto;            ///< Número que identifica o pasto (0 = não cadastrado).
    std::string nome;       ///< Nome do pasto.
    std::string localizacao;///< Localização do pasto.
    int capacidade;         ///< Máximo de animais que o pasto comporta.
    int idPropriedade;      ///< Fazenda a que o pasto pertence.

public:
    /**
     * @brief Construtor padrão.
     *
     * Cria um pasto vazio (id 0, textos vazios, capacidade 0 e propriedade 0).
     */
    Pasto();

    /**
     * @brief Cria um pasto a partir dos dados informados.
     * @param id            Número que identifica o pasto.
     * @param dados         Nome, localização e capacidade do pasto.
     * @param idPropriedade Fazenda a que o pasto pertence.
     */
    Pasto(int id, const DadosPasto& dados, int idPropriedade);

    /** @brief Número que identifica o pasto (0 se ainda não cadastrado). */
    int id() const;

    /** @brief Nome do pasto. */
    std::string name() const;

    /** @brief Localização do pasto. */
    std::string location() const;

    /** @brief Máximo de animais que o pasto comporta. */
    int capacity() const;

    /** @brief Fazenda a que o pasto pertence. */
    int propertyId() const;
};

#endif // PASTO_H