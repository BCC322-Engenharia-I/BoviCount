/**
 * @file Registro.cpp
 * @brief Implementação da classe Registro.
 * @see Registro.hpp
 */

#include "historico/Registro.hpp"

Registro::Registro()
    : idRegistro(0), dataHora(), idOperador(0), idPasto(0),
      totalAutomatico(0), totalConfirmado(0), caminhoImagem() {}

Registro::Registro(int id, const DataHora& dataHora, int idOperador, int idPasto,
                   int totalAutomatico, int totalConfirmado, const std::string& caminhoImagem)
    : idRegistro(id), dataHora(dataHora), idOperador(idOperador), idPasto(idPasto),
      totalAutomatico(totalAutomatico), totalConfirmado(totalConfirmado),
      caminhoImagem(caminhoImagem) {}

int Registro::id() const { return idRegistro; }

DataHora Registro::dateTime() const { return dataHora; }

int Registro::operatorId() const { return idOperador; }

int Registro::pastureId() const { return idPasto; }

int Registro::automaticTotal() const { return totalAutomatico; }

int Registro::confirmedTotal() const { return totalConfirmado; }

std::string Registro::imagePath() const { return caminhoImagem; }
