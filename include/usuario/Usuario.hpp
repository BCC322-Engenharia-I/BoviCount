/**
 * @file Usuario.hpp
 * @brief Declaração da classe Usuario.
 */
#ifndef USUARIO_H
#define USUARIO_H

#include <string>

/**
 * @brief Uma pessoa que usa o sistema.
 *
 * Guarda o identificador do usuário, o papel que ele exerce e a fazenda
 * (propriedade) a que pertence. É um objeto de valor: pode ser copiado
 * livremente.
 *
 * @see Sessao
 */
class Usuario {
private:
    int idUsuario;          ///< Número que identifica o usuário.
    std::string papel;      ///< Papel do usuário ("Gestor", "Funcionário" ou "Administrador").
    int idPropriedade;      ///< Identificador da fazenda a que o usuário pertence.

public:
    /**
     * @brief Construtor padrão.
     *
     * Cria um usuário vazio: id 0, papel vazio e propriedade 0.
     */
    Usuario();

    /**
     * @brief Cria um usuário com os dados informados.
     * @param id            Número que identifica o usuário.
     * @param papel         Papel do usuário: "Gestor", "Funcionário" ou "Administrador".
     * @param idPropriedade Identificador da fazenda a que o usuário pertence.
     */
    Usuario(int id, const std::string& papel, int idPropriedade);

    /** @brief Construtor de cópia: copia id, papel e propriedade de outro Usuario. */
    Usuario(const Usuario&) = default;

    /** @brief Atribuição por cópia: copia id, papel e propriedade de outro Usuario. */
    Usuario& operator=(const Usuario&) = default;

    /** @brief Destrutor. */
    ~Usuario() = default;

    /**
     * @brief Número que identifica o usuário.
     * @return O identificador do usuário.
     */
    int id() const;

    /**
     * @brief Papel exercido pelo usuário.
     * @return "Gestor", "Funcionário" ou "Administrador".
     */
    std::string role() const;

    /**
     * @brief Fazenda a que o usuário pertence.
     * @return O identificador da propriedade.
     */
    int propertyId() const;
};

#endif // USUARIO_H