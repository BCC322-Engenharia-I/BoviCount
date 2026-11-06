#include "Pasto.hpp"

Pasto::Pasto() : idPasto(0), nome(), localizacao(), capacidade(0), idPropriedade(0) {}

Pasto::Pasto(int id, const DadosPasto& dados, int idPropriedade)
    : idPasto(id),
      nome(dados.name()),
      localizacao(dados.location()),
      capacidade(dados.capacity()),
      idPropriedade(idPropriedade) {}

int Pasto::id() const { return idPasto; }

std::string Pasto::name() const { return nome; }

std::string Pasto::location() const { return localizacao; }

int Pasto::capacity() const { return capacidade; }

int Pasto::propertyId() const { return idPropriedade; }