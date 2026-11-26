/**
 * @file test_unit_pasto.cpp
 * @brief Teste unitário da classe Pasto.
 *
 * Usa apenas Pasto e DadosPasto (necessário para construir um Pasto): nenhum
 * serviço ou repositório participa. A classe TestePasto é @c friend de Pasto e
 * confere o estado interno diretamente, sem passar pelos getters.
 * Compile com `make compile-test-unit-pasto` e execute com `make run-test-unit-pasto`.
 */
#include <cassert>
#include <iostream>
#include "pasto/Pasto.hpp"

/** @brief Acesso aos membros privados de Pasto (declarada @c friend em Pasto). */
class TestePasto {
public:
    /** @brief Um Pasto padrão é "ainda não cadastrado": id 0, textos vazios, números 0. */
    static void construtorPadrao() {
        Pasto p;
        assert(p.idPasto == 0);
        assert(p.nome.empty());
        assert(p.localizacao.empty());
        assert(p.capacidade == 0);
        assert(p.idPropriedade == 0);
    }

    /** @brief O construtor com dados guarda id, nome, localização, capacidade e fazenda. */
    static void construtorComDados() {
        Pasto p(7, DadosPasto("Pasto Fundo", "Divisa Norte", 200), 3);
        assert(p.idPasto == 7);
        assert(p.nome == "Pasto Fundo");
        assert(p.localizacao == "Divisa Norte");
        assert(p.capacidade == 200);
        assert(p.idPropriedade == 3);
    }

    /** @brief Os getters devolvem o que está guardado. */
    static void getters() {
        Pasto p(7, DadosPasto("Pasto Fundo", "Divisa Norte", 200), 3);
        assert(p.id() == 7);
        assert(p.name() == "Pasto Fundo");
        assert(p.location() == "Divisa Norte");
        assert(p.capacity() == 200);
        assert(p.propertyId() == 3);
    }

    /** @brief A cópia tem os mesmos valores e é independente do original. */
    static void construtorDeCopia() {
        Pasto original(7, DadosPasto("Pasto Fundo", "Divisa Norte", 200), 3);
        Pasto copia(original);
        assert(copia.idPasto == 7);
        assert(copia.nome == "Pasto Fundo");
        assert(copia.localizacao == "Divisa Norte");
        assert(copia.capacidade == 200);
        assert(copia.idPropriedade == 3);

        original.nome = "Outro Nome";
        original.capacidade = 1;
        assert(copia.nome == "Pasto Fundo");
        assert(copia.capacidade == 200);
    }

    /** @brief A atribuição copia todos os campos, independe do original e aceita auto-atribuição. */
    static void atribuicao() {
        Pasto original(7, DadosPasto("Pasto Fundo", "Divisa Norte", 200), 3);
        Pasto destino;
        destino = original;
        assert(destino.idPasto == 7);
        assert(destino.nome == "Pasto Fundo");
        assert(destino.localizacao == "Divisa Norte");
        assert(destino.capacidade == 200);
        assert(destino.idPropriedade == 3);

        original.localizacao = "Mudou";
        assert(destino.localizacao == "Divisa Norte");

        Pasto& mesmo = destino;
        destino = mesmo;
        assert(destino.nome == "Pasto Fundo");
        assert(destino.idPasto == 7);
    }
};

/**
 * @brief Executa o teste unitário de Pasto.
 * @return 0 se todos passarem (caso contrário o @c assert aborta o programa).
 */
int main() {
    std::cout << "===== TESTE UNITARIO: PASTO =====" << std::endl;
    TestePasto::construtorPadrao();
    TestePasto::construtorComDados();
    TestePasto::getters();
    TestePasto::construtorDeCopia();
    TestePasto::atribuicao();
    std::cout << "  test_unit_pasto: OK" << std::endl;
    std::cout << "=================================" << std::endl;
    return 0;
}
