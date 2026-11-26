/**
 * @file DadosPasto.hpp
 * @brief Declaração da classe DadosPasto.
 */
#ifndef DADOS_PASTO_H
#define DADOS_PASTO_H

#include <string>

/**
 * @brief Dados que o Gestor informa ao cadastrar ou editar um pasto.
 *
 * Objeto de valor sem validação própria: as conferências (campos
 * preenchidos, capacidade positiva, nome único etc.) são feitas por
 * ServicoPastos.
 *
 * @see ServicoPastos::create
 * @see ServicoPastos::update
 */
class DadosPasto {
private:
    std::string nome;           ///< Nome do pasto.
    std::string localizacao;    ///< Localização do pasto na fazenda.
    int capacidade;             ///< Máximo de animais que o pasto comporta.

public:
    /** @brief Construtor padrão: nome e localização vazios, capacidade 0. */
    DadosPasto();

    /**
     * @brief Cria os dados de um pasto.
     * @param nome        Nome do pasto.
     * @param localizacao Localização do pasto.
     * @param capacidade  Máximo de animais que o pasto comporta.
     */
    DadosPasto(const std::string& nome, const std::string& localizacao, int capacidade);

    /** @brief Construtor de cópia: copia nome, localização e capacidade de outro DadosPasto. */
    DadosPasto(const DadosPasto&) = default;

    /** @brief Atribuição por cópia: copia nome, localização e capacidade de outro DadosPasto. */
    DadosPasto& operator=(const DadosPasto&) = default;

    /** @brief Destrutor. */
    ~DadosPasto() = default;

    /** @brief Nome do pasto. */
    std::string name() const;

    /** @brief Localização do pasto. */
    std::string location() const;

    /** @brief Máximo de animais que o pasto comporta. */
    int capacity() const;
};

#endif // DADOS_PASTO_H