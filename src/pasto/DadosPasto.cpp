/**
 * @file DadosPasto.cpp
 * @brief Implementação da classe DadosPasto.
 */
#include "pasto/DadosPasto.hpp"

/** @copydoc DadosPasto::DadosPasto(const std::string&, const std::string&, int) */
DadosPasto::DadosPasto(const std::string& nome, const std::string& localizacao, int capacidade)
    : nome(nome), localizacao(localizacao), capacidade(capacidade) {}

/** @copydoc DadosPasto::name() const */
std::string DadosPasto::name() const { return nome; }

/** @copydoc DadosPasto::location() const */
std::string DadosPasto::location() const { return localizacao; }

/** @copydoc DadosPasto::capacity() const */
int DadosPasto::capacity() const { return capacidade; }