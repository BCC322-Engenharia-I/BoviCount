#include <cassert>
#include <iostream>
#include <stdexcept>
#include "PastoPlano.hpp"
#include "PastoMorro.hpp"
#include "GestaoPastoImpl.hpp"

static void testeCapacidade() {
    PastoPlano plano("Pasto A", "L1", 10.0);
    PastoMorro morro("Pasto B", "L2", 10.0);

    assert(plano.getNome() == "Pasto A");
    assert(plano.calcularCapacidade() == 20);
    assert(morro.calcularCapacidade() == 10);

    // Polimorfismo
    Pasto* base = &plano;
    assert(base->calcularCapacidade() == 20);
}

static void testeFormaCanonica() {
    PastoPlano original("Pasto A", "L1", 5.0);
    PastoPlano copia(original);
    assert(copia.getNome() == "Pasto A");
    assert(copia.calcularCapacidade() == original.calcularCapacidade());

    PastoPlano atribuido;
    atribuido = original;
    assert(atribuido.getNome() == "Pasto A");
    assert(atribuido.calcularCapacidade() == 10);

    PastoMorro padrao;
    assert(padrao.calcularCapacidade() == 0);
}

static void testeAreaNegativa() {
    bool lancou = false;
    try {
        PastoPlano invalido("X", "L", -1.0);
    } catch (const std::invalid_argument&) {
        lancou = true;
    }
    assert(lancou);
}

static void testeGestao() {
    PastoPlano p1("A", "L1", 1.0);
    PastoMorro p2("B", "L2", 1.0);
    GestaoPastoImpl gestao;

    gestao.cadastrarPasto(&p1);
    gestao.cadastrarPasto(&p2);
    gestao.cadastrarPasto(&p1); // duplicado ignorado
    assert(gestao.listarPastos().size() == 2);

    gestao.removerPasto(&p1);
    assert(gestao.listarPastos().size() == 1);
    assert(gestao.listarPastos()[0] == &p2);

    GestaoPastoImpl copia(gestao);
    assert(copia.listarPastos().size() == 1);

    bool lancou = false;
    try {
        gestao.cadastrarPasto(nullptr);
    } catch (const std::invalid_argument&) {
        lancou = true;
    }
    assert(lancou);
}

int main() {
    testeCapacidade();
    testeFormaCanonica();
    testeAreaNegativa();
    testeGestao();
    std::cout << "test_pasto: OK" << std::endl;
    return 0;
}