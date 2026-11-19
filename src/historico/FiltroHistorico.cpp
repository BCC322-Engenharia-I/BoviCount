/**
 * @file FiltroHistorico.cpp
 * @brief Implementação da classe FiltroHistorico.
 * @see FiltroHistorico.hpp
 */

#include "historico/FiltroHistorico.hpp"

FiltroHistorico::FiltroHistorico(const Data& inicio, const Data& fim, int idPasto, int idOperador)
    : inicio(inicio), fim(fim), idPasto(idPasto), idOperador(idOperador) {}

Data FiltroHistorico::start() const { return inicio; }

Data FiltroHistorico::end() const { return fim; }

int FiltroHistorico::pastureId() const { return idPasto; }

int FiltroHistorico::operatorId() const { return idOperador; }
