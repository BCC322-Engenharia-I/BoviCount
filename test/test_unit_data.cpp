#include <cassert>
#include <iostream>
#include "core/Data.hpp"

class TesteData {
public:
    static void construtorPadrao() {
        Data data;
        assert(data.ano == 0 && data.mes == 0 && data.dia == 0);
    }
    static void construtorEGetters() {
        Data data(2026, 11, 26);
        assert(data.ano == 2026 && data.mes == 11 && data.dia == 26);
        assert(data.year() == 2026 && data.month() == 11 && data.day() == 26);
    }
    static void igualdade() {
        Data base(2026, 11, 26);
        assert(base == Data(2026, 11, 26));
        assert(!(base == Data(2025, 11, 26)));
        assert(!(base == Data(2026, 10, 26)));
        assert(!(base == Data(2026, 11, 25)));
    }
    static void ordem() {
        Data base(2026, 11, 26);
        assert(Data(2025, 12, 31) < Data(2026, 1, 1));
        assert(Data(2026, 1, 31) < Data(2026, 2, 1));
        assert(Data(2026, 11, 25) < base);
        assert(!(base < Data(2025, 12, 31)));
        assert(!(base < Data(2026, 1, 31)));
        assert(!(base < Data(2026, 11, 25)));
        assert(!(base < Data(2026, 11, 26)));
    }
    static void copiaEAtribuicao() {
        Data original(2025, 5, 10);
        Data copia(original);
        Data atribuida;
        atribuida = original;
        assert(copia.ano == 2025 && copia.mes == 5 && copia.dia == 10);
        assert(atribuida.ano == 2025 && atribuida.mes == 5 && atribuida.dia == 10);
        original = Data(2000, 1, 1);
        assert(copia.ano == 2025 && atribuida.ano == 2025);
    }
};

int main() {
    std::cout << "===== TESTE UNITARIO: DATA =====" << std::endl;
    TesteData::construtorPadrao();
    TesteData::construtorEGetters();
    TesteData::igualdade();
    TesteData::ordem();
    TesteData::copiaEAtribuicao();
    std::cout << "  test_unit_data: OK" << std::endl;
    std::cout << "================================" << std::endl;
    return 0;
}
