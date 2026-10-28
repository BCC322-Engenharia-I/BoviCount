#ifndef PASTO_PLANO_H
#define PASTO_PLANO_H

#include "Pasto.hpp"

class PastoPlano : public Pasto {
public:
    PastoPlano();
    PastoPlano(const std::string& nome, const std::string& lote, double area);
    PastoPlano(const PastoPlano& outro);
    PastoPlano& operator=(const PastoPlano& outro);
    virtual ~PastoPlano();

    int calcularCapacidade() const override;
};

#endif // PASTO_PLANO_H