/**
 * @file Usuario.cpp
 * @brief Implementação da classe Usuario.
 */
#include "usuario/Usuario.hpp"

/** @copydoc Usuario::Usuario() */
Usuario::Usuario() : idUsuario(0), papel(), idPropriedade(0) {}

/** @copydoc Usuario::Usuario(int, const std::string&, int) */
Usuario::Usuario(int id, const std::string& papel, int idPropriedade)
    : idUsuario(id), papel(papel), idPropriedade(idPropriedade) {}

/** @copydoc Usuario::id() const */
int Usuario::id() const { return idUsuario; }

/** @copydoc Usuario::role() const */
std::string Usuario::role() const { return papel; }

/** @copydoc Usuario::propertyId() const */
int Usuario::propertyId() const { return idPropriedade; }