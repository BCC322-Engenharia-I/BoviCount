#ifndef GESTAO_PASTO_IMPL_H
#define GESTAO_PASTO_IMPL_H

#include <vector>
#include "GestaoPasto.hpp"

// Guarda ponteiros NÃO proprietários: o chamador mantém o ciclo de vida dos pastos.
// A cópia desta classe é rasa (copia os ponteiros).
class GestaoPastoImpl : public GestaoPasto {
private:
    std::vector<Pasto*> pastos;

public:
    GestaoPastoImpl();
    GestaoPastoImpl(const GestaoPastoImpl& outro);
    GestaoPastoImpl& operator=(const GestaoPastoImpl& outro);
    virtual ~GestaoPastoImpl();

    void cadastrarPasto(Pasto* p) override;
    void removerPasto(Pasto* p) override;
    std::vector<Pasto*> listarPastos() const override;
};

#endif // GESTAO_PASTO_IMPL_H