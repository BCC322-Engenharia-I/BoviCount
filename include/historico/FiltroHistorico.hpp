#ifndef FILTROHISTORICO_HPP
#define FILTROHISTORICO_HPP

#include "core/Data.hpp"

// Criterio para buscar contagens salvas: periodo e, se quiser, pasto e funcionario.
class FiltroHistorico {
public:
    FiltroHistorico(const Data& inicio, const Data& fim,
                    int idPasto = 0,        // 0 = todos os pastos
                    int idOperador = 0);    // 0 = todos os funcionarios

    Data start() const;
    Data end() const;
    int pastureId() const;
    int operatorId() const;

private:
    Data inicio;
    Data fim;
    int idPasto;
    int idOperador;
};

#endif
