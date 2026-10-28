#include "Pasto.hpp"
#include <stdexcept>

Pasto::Pasto() : nome(), lote(), area(0.0) {}

Pasto::Pasto(const std::string& nome, const std::string& lote, double area)
    : nome(nome), lote(lote), area(area) {
    if (area < 0.0) {
        throw std::invalid_argument("Pasto: a area nao pode ser negativa");
    }
}

Pasto::Pasto(const Pasto& novo)
    : nome(novo.nome), lote(novo.lote), area(novo.area) {}

Pasto& Pasto::operator=(const Pasto& novo) {
    if (this != &novo) {
        nome = novo.nome;
        lote = novo.lote;
        area = novo.area;
    }
    return *this;
}

Pasto::~Pasto() {}

std::string Pasto::getNome() const {
    return nome;
}