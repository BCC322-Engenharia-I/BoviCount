#ifndef PASTO_MORRO_H
#define PASTO_MORRO_H

#include "Pasto.hpp"

class PastoMorro : public Pasto {
public:
    PastoMorro();
    PastoMorro(const std::string& nome, const std::string& lote, double area);
    PastoMorro(const PastoMorro& outro);
    PastoMorro& operator=(const PastoMorro& outro);
    virtual ~PastoMorro();

    int calcularCapacidade() const override;
};

#endif // PASTO_MORRO_H