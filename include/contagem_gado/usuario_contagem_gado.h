/**
 * @file usuario_contagem_gado.h
 * @brief Declaração dos dados de usuário e sessão usados pela contagem.
 */

#ifndef CONTAGEM_GADO_USUARIO_H
#define CONTAGEM_GADO_USUARIO_H

#include <string>

namespace contagem_gado {

/**
 * @class Usuario
 * @brief Dados do usuário relevantes para a autorização da contagem.
 *
 * Guarda identificador, papel e propriedade. Esta classe representa o
 * usuário no módulo contagem_gado, separado do módulo geral de usuários.
 */
class Usuario {
private:
    /** @brief Identificador do usuário. */
    int id_;
    /** @brief Papel informado para o usuário. */
    std::string papel_;
    /** @brief Identificador da propriedade associada. */
    int idPropriedade_;

public:
    /**
     * @brief Cria os dados de um usuário do módulo de contagem.
     * @param id Identificador do usuário.
     * @param papel Papel do usuário, como Gestor, Funcionário ou Administrador.
     * @param idPropriedade Identificador da propriedade.
     */
    Usuario(int id, const std::string& papel, int idPropriedade);
    /** @brief Devolve o identificador do usuário. */
    int id() const;
    /** @brief Devolve o papel do usuário. */
    std::string role() const;
    /** @brief Devolve o identificador da propriedade. */
    int propertyId() const;
};

/**
 * @class Sessao
 * @brief Mantém uma cópia do usuário associado à operação de contagem.
 * @see Usuario
 */
class Sessao {
private:
    /** @brief Usuário associado à sessão. */
    Usuario usuario_;

public:
    /**
     * @brief Cria uma sessão com os dados do usuário.
     * @param usuario Usuário que será associado à sessão.
     */
    explicit Sessao(const Usuario& usuario);
    /** @brief Devolve uma referência constante ao usuário da sessão. */
    const Usuario& user() const;
};

} // namespace contagem_gado

#endif
