/**
 * @file Sessao.hpp
 * @brief Declaração da classe Sessao.
 */
#ifndef SESSAO_H
#define SESSAO_H

#include "usuario/Usuario.hpp"

/**
 * @brief Guarda quem está usando o sistema no momento.
 *
 * Todo serviço consulta a Sessao para saber o que a pessoa pode fazer
 * (permissões por papel) e a qual fazenda os dados pertencem.
 *
 * @see Usuario
 * @see ServicoPastos
 */
class Sessao {
private:
    Usuario usuario;    ///< Usuário autenticado nesta sessão (guardado por cópia).

public:
    /**
     * @brief Abre uma sessão para o usuário informado.
     * @param usuario Usuário que está usando o sistema.
     */
    explicit Sessao(const Usuario& usuario);

    /**
     * @brief Usuário da sessão.
     * @return Referência constante ao usuário; válida enquanto a Sessao existir.
     */
    const Usuario& user() const;
};

#endif // SESSAO_H