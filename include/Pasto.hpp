#ifndef PASTO_H
#define PASTO_H

#include <string>
#include "DadosPasto.hpp"

// Um pasto ja cadastrado: os dados informados mais o numero que o identifica
// e a fazenda a que pertence.
class Pasto {
private:
    int idPasto;
    std::string nome;
    std::string localizacao;
    int capacidade;
    int idPropriedade;

public:
    Pasto();
    Pasto(int id, const DadosPasto& dados, int idPropriedade);

    int id() const;
    std::string name() const;
    std::string location() const;
    int capacity() const;
    int propertyId() const;   // fazenda a que o pasto pertence
};

#endif // PASTO_H