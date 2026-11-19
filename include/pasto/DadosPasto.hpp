/**
 * @file Pasto.cpp
 * @brief Implementação da classe Pasto.
 */
#include "pasto/Pasto.hpp"

/** @copydoc Pasto::Pasto() */
Pasto::Pasto() : idPasto(0), nome(), localizacao(), capacidade(0), idPropriedade(0) {}

/** @copydoc Pasto::Pasto(int, const DadosPasto&, int) */
Pasto::Pasto(int id, const DadosPasto& dados, int idPropriedade)
    : idPasto(id),
      nome(dados.name()),
      localizacao(dados.location()),
      capacidade(dados.capacity()),
      idPropriedade(idPropriedade) {}

/** @copydoc Pasto::id() const */
int Pasto::id() const { return idPasto; }

/** @copydoc Pasto::name() const */
std::string Pasto::name() const { return nome; }

/** @copydoc Pasto::location() const */
std::string Pasto::location() const { return localizacao; }

/** @copydoc Pasto::capacity() const */
int Pasto::capacity() const { return capacidade; }

/** @copydoc Pasto::propertyId() const */
int Pasto::propertyId() const { return idPropriedade; }