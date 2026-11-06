#include "DadosPasto.hpp"

DadosPasto::DadosPasto(const std::string& nome, const std::string& localizacao, int capacidade)
    : nome(nome), localizacao(localizacao), capacidade(capacidade) {}

std::string DadosPasto::name() const { return nome; }

std::string DadosPasto::location() const { return localizacao; }

int DadosPasto::capacity() const { return capacidade; }