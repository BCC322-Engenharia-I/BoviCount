#include "Sessao.hpp"

Sessao::Sessao(const Usuario& usuario) : usuario(usuario) {}

const Usuario& Sessao::user() const {
    return usuario;
}