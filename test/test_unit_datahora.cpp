#include <cassert>
#include <ctime>
#include <iostream>
#include "core/DataHora.hpp"

class TesteDataHora {
    static bool corresponde(const DataHora& momento, std::time_t t) {
        std::tm* p = std::localtime(&t);
        return momento.data == Data(p->tm_year + 1900, p->tm_mon + 1, p->tm_mday)
            && momento.hora == p->tm_hour
            && momento.minuto == p->tm_min
            && momento.segundo == p->tm_sec;
    }
public:
    static void construtorPadrao() {
        DataHora momento;
        assert(momento.data == Data());
        assert(momento.hora == 0 && momento.minuto == 0 && momento.segundo == 0);
    }
    static void construtorEGetters() {
        Data dia(2026, 11, 26);
        DataHora momento(dia, 14, 35, 42);
        assert(momento.data == dia);
        assert(momento.hora == 14 && momento.minuto == 35 && momento.segundo == 42);
        assert(momento.date() == dia);
        assert(momento.hour() == 14 && momento.minute() == 35 && momento.second() == 42);
    }
    static void copiaEAtribuicao() {
        DataHora original(Data(2025, 4, 3), 8, 9, 10);
        DataHora copia(original);
        DataHora atribuida;
        atribuida = original;
        assert(copia.data == Data(2025, 4, 3) && copia.hora == 8 && copia.minuto == 9 && copia.segundo == 10);
        assert(atribuida.data == Data(2025, 4, 3) && atribuida.hora == 8 && atribuida.minuto == 9 && atribuida.segundo == 10);
        original = DataHora(Data(2000, 1, 1), 1, 2, 3);
        assert(copia.hora == 8 && atribuida.hora == 8);
    }
    static void agoraUsaRelogioLocal() {
        std::time_t antes = std::time(0);
        DataHora momento = DataHora::agora();
        std::time_t depois = std::time(0);
        assert(corresponde(momento, antes) || corresponde(momento, depois));
    }
};

int main() {
    std::cout << "===== TESTE UNITARIO: DATAHORA =====" << std::endl;
    TesteDataHora::construtorPadrao();
    TesteDataHora::construtorEGetters();
    TesteDataHora::copiaEAtribuicao();
    TesteDataHora::agoraUsaRelogioLocal();
    std::cout << "  test_unit_datahora: OK" << std::endl;
    std::cout << "====================================" << std::endl;
    return 0;
}
