/**
 * @file test_unit_retangulo.cpp
 * @brief Teste unitário da classe Retangulo.
 *
 * Usa apenas Retangulo. A classe TesteRetangulo é @c friend de Retangulo e
 * confere o estado interno diretamente.
 * Compile com `make compile-test-unit-retangulo` e execute com `make run-test-unit-retangulo`.
 */
#include <cassert>
#include <iostream>
#include "contagem/Retangulo.hpp"

/** @brief Acesso aos membros privados de Retangulo (declarada @c friend em Retangulo). */
class TesteRetangulo {
public:
    /** @brief Um Retangulo padrão está na posição (0, 0) com tamanho 0 x 0. */
    static void construtorPadrao() {
        Retangulo r;
        assert(r.posX == 0);
        assert(r.posY == 0);
        assert(r.largura == 0);
        assert(r.altura == 0);
    }

    /** @brief O construtor guarda posição e tamanho. */
    static void construtorComValores() {
        Retangulo r(10, 20, 30, 40);
        assert(r.posX == 10);
        assert(r.posY == 20);
        assert(r.largura == 30);
        assert(r.altura == 40);
    }

    /** @brief Os getters devolvem x, y, largura e altura sem confundir os campos. */
    static void getters() {
        Retangulo r(10, 20, 30, 40);
        assert(r.x() == 10);
        assert(r.y() == 20);
        assert(r.width() == 30);
        assert(r.height() == 40);
    }

    /** @brief A cópia tem os mesmos valores e é independente do original. */
    static void construtorDeCopia() {
        Retangulo original(1, 2, 3, 4);
        Retangulo copia(original);
        assert(copia.posX == 1 && copia.posY == 2);
        assert(copia.largura == 3 && copia.altura == 4);

        original.posX = 99;
        original.altura = 99;
        assert(copia.posX == 1);
        assert(copia.altura == 4);
    }

    /** @brief A atribuição copia todos os campos, independe do original e aceita auto-atribuição. */
    static void atribuicao() {
        Retangulo original(1, 2, 3, 4);
        Retangulo destino;
        destino = original;
        assert(destino.posX == 1 && destino.posY == 2);
        assert(destino.largura == 3 && destino.altura == 4);

        original.largura = 99;
        assert(destino.largura == 3);

        Retangulo& mesmo = destino;
        destino = mesmo;
        assert(destino.posX == 1 && destino.altura == 4);
    }
};

/**
 * @brief Executa o teste unitário de Retangulo.
 * @return 0 se todos passarem (caso contrário o @c assert aborta o programa).
 */
int main() {
    std::cout << "===== TESTE UNITARIO: RETANGULO =====" << std::endl;
    TesteRetangulo::construtorPadrao();
    TesteRetangulo::construtorComValores();
    TesteRetangulo::getters();
    TesteRetangulo::construtorDeCopia();
    TesteRetangulo::atribuicao();
    std::cout << "  test_unit_retangulo: OK" << std::endl;
    std::cout << "=====================================" << std::endl;
    return 0;
}
