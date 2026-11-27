#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include "core/Regra.hpp"

static_assert(std::is_abstract<Regra<int> >::value, "Regra deve ser abstrata");
static_assert(std::has_virtual_destructor<Regra<int> >::value, "Regra deve ter destrutor virtual");

class TesteRegra {
    class RegraPositiva : public Regra<int> {
    public:
        bool allows(const int& valor) const override { return valor > 0; }
    };
    class RegraTextoNaoVazio : public Regra<std::string> {
    public:
        bool allows(const std::string& texto) const override { return !texto.empty(); }
    };
    class RegraComAviso : public Regra<int> {
        bool* destruida;
    public:
        explicit RegraComAviso(bool* flag) : destruida(flag) {}
        ~RegraComAviso() { *destruida = true; }
        bool allows(const int&) const override { return true; }
    };
public:
    static void aceitaERecusa() {
        RegraPositiva regra;
        assert(regra.allows(5));
        assert(!regra.allows(0));
        assert(!regra.allows(-2));
    }
    static void funcionaPorPolimorfismo() {
        RegraPositiva concreta;
        const Regra<int>& interface = concreta;
        const Regra<int>* ponteiro = &concreta;
        assert(interface.allows(1) && !interface.allows(-1));
        assert(ponteiro->allows(1) && !ponteiro->allows(-1));
    }
    static void outroTipo() {
        RegraTextoNaoVazio regra;
        const Regra<std::string>& interface = regra;
        assert(interface.allows("pasto 1"));
        assert(!interface.allows(""));
    }
    static void destrutorVirtual() {
        bool destruida = false;
        Regra<int>* regra = new RegraComAviso(&destruida);
        assert(regra->allows(0));
        delete regra;
        assert(destruida);
    }
};

int main() {
    std::cout << "===== TESTE UNITARIO: REGRA =====" << std::endl;
    TesteRegra::aceitaERecusa();
    TesteRegra::funcionaPorPolimorfismo();
    TesteRegra::outroTipo();
    TesteRegra::destrutorVirtual();
    std::cout << "  test_unit_regra: OK" << std::endl;
    std::cout << "=================================" << std::endl;
    return 0;
}
