/**
 * @file usuario_contagem_gado.cpp
 * @brief Implementação dos dados de usuário e de sua sessão na contagem.
 * @see usuario_contagem_gado.h
 */

#include "contagem_gado/usuario_contagem_gado.h"

namespace contagem_gado {

// Implementações dos construtores e acessores declarados no cabeçalho.
Usuario::Usuario(int id, const std::string& papel, int idPropriedade)
    : id_(id), papel_(papel), idPropriedade_(idPropriedade) {}

int Usuario::id() const { return id_; }
std::string Usuario::role() const { return papel_; }
int Usuario::propertyId() const { return idPropriedade_; }

Sessao::Sessao(const Usuario& usuario) : usuario_(usuario) {}
const Usuario& Sessao::user() const { return usuario_; }

} // namespace contagem_gado
