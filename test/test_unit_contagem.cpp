/**
 * @file test_unit_contagem.cpp
 * @brief Teste unitário da classe Contagem.
 *
 * Usa apenas Contagem e os tipos de que ela é composta (Imagem, Retangulo e
 * Colecao): nenhum serviço ou motor de visão participa. A classe TesteContagem é
 * @c friend de Contagem e confere o estado interno diretamente.
 * Compile com `make compile-test-unit-contagem` e execute com `make run-test-unit-contagem`.
 */
#include <cassert>
#include <iostream>
#include "contagem/Contagem.hpp"

/** @brief Acesso aos membros privados de Contagem (declarada @c friend em Contagem). */
class TesteContagem {
public:
    /** @brief Monta uma coleção com @p n marcações distintas (a i-ésima fica em x = i). */
    static Colecao<Retangulo> caixas(int n) {
        Colecao<Retangulo> c;
        for (int i = 0; i < n; ++i) {
            c.add(Retangulo(i, i * 2, 10, 10));
        }
        return c;
    }

    /** @brief Uma Contagem padrão não tem foto nem marcações e o tempo é 0. */
    static void construtorPadrao() {
        Contagem c;
        assert(c.foto.path().empty());
        assert(c.marcacoes.size() == 0);
        assert(c.segundos == 0.0);
        assert(c.total() == 0);
    }

    /** @brief O construtor guarda a foto, as marcações e o tempo da análise. */
    static void construtorComResultado() {
        Contagem c(Imagem("foto.jpg"), caixas(3), 1.5);
        assert(c.foto.path() == "foto.jpg");
        assert(c.marcacoes.size() == 3);
        assert(c.segundos == 1.5);
    }

    /** @brief O total é o número de marcações; foto sem animais é uma Contagem válida com total 0. */
    static void total() {
        assert(Contagem(Imagem("foto.jpg"), caixas(185), 2.0).total() == 185);
        Contagem vazia(Imagem("foto.jpg"), caixas(0), 0.3);
        assert(vazia.total() == 0);
        assert(!vazia.foto.path().empty());   // tem foto: nao e o mesmo que Contagem()
    }

    /** @brief Os getters devolvem tempo, foto e marcações, na mesma ordem em que foram dadas. */
    static void getters() {
        Contagem c(Imagem("foto.jpg"), caixas(3), 1.5);
        assert(c.processingSeconds() == 1.5);
        assert(c.image().path() == "foto.jpg");

        Colecao<Retangulo> marcas = c.boxes();
        assert(marcas.size() == 3);
        Iterador<Retangulo> it = marcas.iterator();
        for (int i = 0; i < 3; ++i) {
            assert(it.hasNext());
            const Retangulo& r = it.next();
            assert(r.x() == i && r.y() == i * 2);
        }
        assert(!it.hasNext());
    }

    /** @brief A cópia tem os mesmos valores e é independente do original. */
    static void construtorDeCopia() {
        Contagem original(Imagem("foto.jpg"), caixas(3), 1.5);
        Contagem copia(original);
        assert(copia.foto.path() == "foto.jpg");
        assert(copia.marcacoes.size() == 3);
        assert(copia.segundos == 1.5);

        original.segundos = 9.0;
        original.foto = Imagem("outra.png");
        original.marcacoes.add(Retangulo());
        assert(copia.segundos == 1.5);
        assert(copia.foto.path() == "foto.jpg");
        assert(copia.marcacoes.size() == 3);
        assert(copia.total() == 3);
    }

    /** @brief A atribuição copia todos os campos, independe do original e aceita auto-atribuição. */
    static void atribuicao() {
        Contagem original(Imagem("foto.jpg"), caixas(3), 1.5);
        Contagem destino;
        destino = original;
        assert(destino.foto.path() == "foto.jpg");
        assert(destino.marcacoes.size() == 3);
        assert(destino.segundos == 1.5);

        original.marcacoes.add(Retangulo());
        original.segundos = 9.0;
        assert(destino.total() == 3);
        assert(destino.segundos == 1.5);

        Contagem& mesma = destino;
        destino = mesma;
        assert(destino.total() == 3);
        assert(destino.foto.path() == "foto.jpg");
    }
};

/**
 * @brief Executa o teste unitário de Contagem.
 * @return 0 se todos passarem (caso contrário o @c assert aborta o programa).
 */
int main() {
    std::cout << "===== TESTE UNITARIO: CONTAGEM =====" << std::endl;
    TesteContagem::construtorPadrao();
    TesteContagem::construtorComResultado();
    TesteContagem::total();
    TesteContagem::getters();
    TesteContagem::construtorDeCopia();
    TesteContagem::atribuicao();
    std::cout << "  test_unit_contagem: OK" << std::endl;
    std::cout << "====================================" << std::endl;
    return 0;
}
