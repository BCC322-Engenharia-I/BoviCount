#include <iostream>
#include "PastoPlano.hpp"
#include "PastoMorro.hpp"
#include "GestaoPastoImpl.hpp"

int main() {
    PastoPlano p1("Pasto A", "L1", 10.0);
    PastoMorro p2("Pasto B", "L2", 10.0);
    GestaoPastoImpl gestaoPasto;

    gestaoPasto.cadastrarPasto(&p1);
    gestaoPasto.cadastrarPasto(&p2);

    std::vector<Pasto*> pastos = gestaoPasto.listarPastos();
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        std::cout << pastos[i]->getNome()
                  << " - capacidade: " << pastos[i]->calcularCapacidade()
                  << " cabecas" << std::endl;
    }

    return 0;
}