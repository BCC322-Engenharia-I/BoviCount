#include "Usuario.hpp"

Usuario::Usuario() : idUsuario(0), papel(), idPropriedade(0) {}

Usuario::Usuario(int id, const std::string& papel, int idPropriedade)
    : idUsuario(id), papel(papel), idPropriedade(idPropriedade) {}

int Usuario::id() const { return idUsuario; }

std::string Usuario::role() const { return papel; }

int Usuario::propertyId() const { return idPropriedade; }