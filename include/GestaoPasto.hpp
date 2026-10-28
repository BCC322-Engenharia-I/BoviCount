#ifndef GESTAO_PASTO_H
#define GESTAO_PASTO_H

#include <vector>
#include "Pasto.hpp"

// Interface de gestão de pastos.
class GestaoPasto {
public:
    virtual ~GestaoPasto() {}

    virtual void cadastrarPasto(Pasto* p) = 0;
    virtual void removerPasto(Pasto* p) = 0;
    virtual std::vector<Pasto*> listarPastos() const = 0;
};

#endif // GESTAO_PASTO_H