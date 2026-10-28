#include "PastoMorro.hpp"

namespace {
    const double CABECAS_POR_HECTARE = 1.0;
}

PastoMorro::PastoMorro() : Pasto() {}

PastoMorro::PastoMorro(const std::string& nome, const std::string& lote, double area)
    : Pasto(nome, lote, area) {}

PastoMorro::PastoMorro(const PastoMorro& outro) : Pasto(outro) {}

PastoMorro& PastoMorro::operator=(const PastoMorro& outro) {
    Pasto::operator=(outro);
    return *this;
}

PastoMorro::~PastoMorro() {}

int PastoMorro::calcularCapacidade() const {
    return static_cast<int>(area * CABECAS_POR_HECTARE);
}