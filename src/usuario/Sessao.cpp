/**
 * @file Sessao.cpp
 * @brief Implementação da classe Sessao.
 */
#include "usuario/Sessao.hpp"

/** @copydoc Sessao::Sessao(const Usuario&) */
Sessao::Sessao(const Usuario& usuario) : usuario(usuario) {}

/** @copydoc Sessao::user() const */
const Usuario& Sessao::user() const {
    return usuario;
}