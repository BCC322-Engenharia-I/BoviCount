/**
 * @file test_unit_imagem.cpp
 * @brief Teste unitário da classe Imagem.
 *
 * Usa apenas Imagem. A classe TesteImagem é @c friend de Imagem e confere o
 * estado interno diretamente. Imagem só guarda o caminho: não abre nem confere
 * o arquivo, então os caminhos usados aqui não precisam existir.
 * Compile com `make compile-test-unit-imagem` e execute com `make run-test-unit-imagem`.
 */
#include <cassert>
#include <iostream>
#include "contagem/Imagem.hpp"

/** @brief Acesso aos membros privados de Imagem (declarada @c friend em Imagem). */
class TesteImagem {
public:
    /** @brief Uma Imagem padrão não tem caminho ("nenhuma foto"). */
    static void construtorPadrao() {
        Imagem i;
        assert(i.caminho.empty());
        assert(i.path().empty());
    }

    /** @brief O construtor guarda o caminho como foi informado, sem validar. */
    static void construtorComCaminho() {
        Imagem i("vistoria_20261006_pasto1.jpg");
        assert(i.caminho == "vistoria_20261006_pasto1.jpg");
        assert(i.path() == "vistoria_20261006_pasto1.jpg");

        Imagem inexistente("nao/existe.png");
        assert(inexistente.path() == "nao/existe.png");
    }

    /** @brief A cópia tem o mesmo caminho e é independente do original. */
    static void construtorDeCopia() {
        Imagem original("a.jpg");
        Imagem copia(original);
        assert(copia.caminho == "a.jpg");

        original.caminho = "b.png";
        assert(copia.caminho == "a.jpg");
    }

    /** @brief A atribuição copia o caminho, independe do original e aceita auto-atribuição. */
    static void atribuicao() {
        Imagem original("a.jpg");
        Imagem destino;
        destino = original;
        assert(destino.caminho == "a.jpg");

        original.caminho = "b.png";
        assert(destino.caminho == "a.jpg");

        Imagem& mesma = destino;
        destino = mesma;
        assert(destino.caminho == "a.jpg");
    }
};

/**
 * @brief Executa o teste unitário de Imagem.
 * @return 0 se todos passarem (caso contrário o @c assert aborta o programa).
 */
int main() {
    std::cout << "===== TESTE UNITARIO: IMAGEM =====" << std::endl;
    TesteImagem::construtorPadrao();
    TesteImagem::construtorComCaminho();
    TesteImagem::construtorDeCopia();
    TesteImagem::atribuicao();
    std::cout << "  test_unit_imagem: OK" << std::endl;
    std::cout << "==================================" << std::endl;
    return 0;
}
