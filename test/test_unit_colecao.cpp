#include <cassert>
#include <iostream>
#include <string>
#include "core/Colecao.hpp"

class TesteColecao {
public:
    static void iniciaVazia() {
        Colecao<int> colecao;
        assert(colecao.itens.empty());
        assert(colecao.size() == 0);
    }
    static void adicionaItens() {
        Colecao<int> colecao;
        colecao.add(10);
        assert(colecao.itens.size() == 1 && colecao.size() == 1);
        colecao.add(20);
        assert(colecao.itens.size() == 2 && colecao.size() == 2);
        assert(colecao.itens[0] == 10 && colecao.itens[1] == 20);
    }
    static void outroTipo() {
        Colecao<std::string> colecao;
        colecao.add("boi");
        colecao.add("vaca");
        assert(colecao.size() == 2 && colecao.itens[1] == "vaca");
    }
    static void iteradorDeColecaoVazia() {
        Colecao<int> colecao;
        assert(!colecao.iterator().hasNext());
    }
    static void iteradorPercorreCopiaDosItens() {
        Colecao<int> colecao;
        colecao.add(3);
        colecao.add(6);
        Iterador<int> iterador = colecao.iterator();
        colecao.add(9);
        assert(iterador.next() == 3 && iterador.next() == 6);
        assert(!iterador.hasNext());
        assert(colecao.size() == 3);
    }
    static void copiaEAtribuicao() {
        Colecao<int> original;
        original.add(7);
        Colecao<int> copia(original);
        Colecao<int> atribuida;
        atribuida.add(1);
        atribuida = original;
        original.add(8);
        assert(copia.itens.size() == 1 && copia.itens[0] == 7);
        assert(atribuida.itens.size() == 1 && atribuida.itens[0] == 7);
        assert(original.itens.size() == 2);
    }
};

int main() {
    std::cout << "===== TESTE UNITARIO: COLECAO =====" << std::endl;
    TesteColecao::iniciaVazia();
    TesteColecao::adicionaItens();
    TesteColecao::outroTipo();
    TesteColecao::iteradorDeColecaoVazia();
    TesteColecao::iteradorPercorreCopiaDosItens();
    TesteColecao::copiaEAtribuicao();
    std::cout << "  test_unit_colecao: OK" << std::endl;
    std::cout << "===================================" << std::endl;
    return 0;
}