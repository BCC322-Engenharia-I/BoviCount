#include "PastoPlano.hpp"

namespace {
    const double CABECAS_POR_HECTARE = 2.0;
}

PastoPlano::PastoPlano() : Pasto() {}

PastoPlano::PastoPlano(const std::string& nome, const std::string& lote, double area)
    : Pasto(nome, lote, area) {}

PastoPlano::PastoPlano(const PastoPlano& outro) : Pasto(outro) {}

PastoPlano& PastoPlano::operator=(const PastoPlano& outro) {
    Pasto::operator=(outro);
    return *this;
}

PastoPlano::~PastoPlano() {}

int PastoPlano::calcularCapacidade() const {
    return static_cast<int>(area * CABECAS_POR_HECTARE);
}