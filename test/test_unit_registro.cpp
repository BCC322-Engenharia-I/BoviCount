/**
 * @file test_unit_registro.cpp
 * @brief Teste unitário da classe Registro.
 *
 * Usa apenas Registro e DataHora (campo do Registro): nenhum serviço ou
 * repositório participa. A classe TesteRegistro é @c friend de Registro e confere
 * o estado interno diretamente.
 * Compile com `make compile-test-unit-registro` e execute com `make run-test-unit-registro`.
 */
#include <cassert>
#include <iostream>
#include "historico/Registro.hpp"

/** @brief Acesso aos membros privados de Registro (declarada @c friend em Registro). */
class TesteRegistro {
public:
    /** @brief Um Registro padrão é "nenhum registro": id 0, totais 0 e sem foto. */
    static void construtorPadrao() {
        Registro r;
        assert(r.idRegistro == 0);
        assert(r.dataHora.date() == Data());
        assert(r.idOperador == 0);
        assert(r.idPasto == 0);
        assert(r.totalAutomatico == 0);
        assert(r.totalConfirmado == 0);
        assert(r.caminhoImagem.empty());
    }

    /** @brief O construtor completo guarda todos os campos. */
    static void construtorCompleto() {
        Registro r(4, DataHora(Data(2026, 10, 6), 14, 30, 5), 5, 2, 180, 200, "foto.jpg");
        assert(r.idRegistro == 4);
        assert(r.dataHora.date() == Data(2026, 10, 6));
        assert(r.dataHora.hour() == 14 && r.dataHora.minute() == 30 && r.dataHora.second() == 5);
        assert(r.idOperador == 5);
        assert(r.idPasto == 2);
        assert(r.totalAutomatico == 180);
        assert(r.totalConfirmado == 200);
        assert(r.caminhoImagem == "foto.jpg");
    }

    /** @brief Os getters devolvem o que está guardado (totais automático e confirmado separados). */
    static void getters() {
        Registro r(4, DataHora(Data(2026, 10, 6), 14, 30, 5), 5, 2, 180, 200, "foto.jpg");
        assert(r.id() == 4);
        assert(r.dateTime().date() == Data(2026, 10, 6));
        assert(r.dateTime().hour() == 14);
        assert(r.operatorId() == 5);
        assert(r.pastureId() == 2);
        assert(r.automaticTotal() == 180);
        assert(r.confirmedTotal() == 200);
        assert(r.imagePath() == "foto.jpg");
    }

    /** @brief A cópia tem os mesmos valores e é independente do original. */
    static void construtorDeCopia() {
        Registro original(4, DataHora(Data(2026, 10, 6), 14, 30, 5), 5, 2, 180, 200, "foto.jpg");
        Registro copia(original);
        assert(copia.idRegistro == 4);
        assert(copia.dataHora.date() == Data(2026, 10, 6));
        assert(copia.idOperador == 5);
        assert(copia.idPasto == 2);
        assert(copia.totalAutomatico == 180);
        assert(copia.totalConfirmado == 200);
        assert(copia.caminhoImagem == "foto.jpg");

        original.totalConfirmado = 999;
        original.caminhoImagem = "outra.png";
        assert(copia.totalConfirmado == 200);
        assert(copia.caminhoImagem == "foto.jpg");
    }

    /** @brief A atribuição copia todos os campos, independe do original e aceita auto-atribuição. */
    static void atribuicao() {
        Registro original(4, DataHora(Data(2026, 10, 6), 14, 30, 5), 5, 2, 180, 200, "foto.jpg");
        Registro destino;
        destino = original;
        assert(destino.idRegistro == 4);
        assert(destino.dataHora.date() == Data(2026, 10, 6));
        assert(destino.idOperador == 5);
        assert(destino.idPasto == 2);
        assert(destino.totalAutomatico == 180);
        assert(destino.totalConfirmado == 200);
        assert(destino.caminhoImagem == "foto.jpg");

        original.idPasto = 77;
        assert(destino.idPasto == 2);

        Registro& mesmo = destino;
        destino = mesmo;
        assert(destino.idRegistro == 4);
        assert(destino.caminhoImagem == "foto.jpg");
    }
};

/**
 * @brief Executa o teste unitário de Registro.
 * @return 0 se todos passarem (caso contrário o @c assert aborta o programa).
 */
int main() {
    std::cout << "===== TESTE UNITARIO: REGISTRO =====" << std::endl;
    TesteRegistro::construtorPadrao();
    TesteRegistro::construtorCompleto();
    TesteRegistro::getters();
    TesteRegistro::construtorDeCopia();
    TesteRegistro::atribuicao();
    std::cout << "  test_unit_registro: OK" << std::endl;
    std::cout << "====================================" << std::endl;
    return 0;
}
