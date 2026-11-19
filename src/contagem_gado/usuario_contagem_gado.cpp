#include "contagem_gado/usuario_contagem_gado.h"

namespace contagem_gado {

Usuario::Usuario(int id, const std::string& papel, int idPropriedade)
    : id_(id), papel_(papel), idPropriedade_(idPropriedade) {}

int Usuario::id() const { return id_; }
std::string Usuario::role() const { return papel_; }
int Usuario::propertyId() const { return idPropriedade_; }

Sessao::Sessao(const Usuario& usuario) : usuario_(usuario) {}
const Usuario& Sessao::user() const { return usuario_; }

} // namespace contagem_gado
