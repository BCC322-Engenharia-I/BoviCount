#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <string>
#include "core/DataHora.hpp"

// Uma contagem salva no historico.
class Registro {
public:
    Registro();
    Registro(int id, const DataHora& dataHora, int idOperador, int idPasto,
             int totalAutomatico, int totalConfirmado, const std::string& caminhoImagem);

    int id() const;
    DataHora dateTime() const;      // quando a contagem foi salva
    int operatorId() const;         // quem estava usando o sistema
    int pastureId() const;          // em qual pasto foi feita
    int automaticTotal() const;     // total calculado pelo sistema
    int confirmedTotal() const;     // total confirmado (ou corrigido) pelo usuario
    std::string imagePath() const;  // foto com as marcacoes, guardada no computador

private:
    int idRegistro;
    DataHora dataHora;
    int idOperador;
    int idPasto;
    int totalAutomatico;
    int totalConfirmado;
    std::string caminhoImagem;
};

#endif
