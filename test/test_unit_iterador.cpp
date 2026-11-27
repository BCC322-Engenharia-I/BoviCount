#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include "core/Iterador.hpp"

static_assert(!std::is_default_constructible<Iterador<int> >::value, "Iterador nao pode ser criado sem lista");

class TesteIterador {
public:
    static void estadoInicial() {
        Iterador<int> iterador(std::vector<int>{4, 8, 12});
        assert(iterador.posicao == 0);
        assert(iterador.itens.size() == 3);
        assert(iterador.hasNext());
    }
    static void percorreTodosOsItens() {
        Iterador<int> iterador(std::vector<int>{4, 8, 12});
        assert(iterador.next() == 4);
        assert(iterador.posicao == 1 && iterador.hasNext());
        assert(iterador.next() == 8);
        assert(iterador.posicao == 2 && iterador.hasNext());
        assert(iterador.next() == 12);
        assert(iterador.posicao == 3 && !iterador.hasNext());
    }
    static void listaVazia() {
        Iterador<int> iterador((std::vector<int>()));
        assert(iterador.itens.empty() && iterador.posicao == 0);
        assert(!iterador.hasNext());
    }
    static void guardaCopiaDaLista() {
        std::vector<int> origem{1, 2};
        Iterador<int> iterador(origem);
        origem.push_back(3);
        origem[0] = 99;
        assert(iterador.itens.size() == 2 && iterador.itens[0] == 1);
    }
    static void copiaPreservaPosicao() {
        Iterador<int> original(std::vector<int>{2, 5, 9});
        assert(original.next() == 2);
        Iterador<int> copia(original);
        assert(copia.posicao == 1 && copia.itens == original.itens);
        assert(copia.next() == 5);
        assert(copia.posicao == 2 && original.posicao == 1);
    }
    static void atribuicao() {
        Iterador<std::string> origem(std::vector<std::string>{"a", "b", "c"});
        origem.next();
        origem.next();
        Iterador<std::string> destino(std::vector<std::string>{"x"});
        destino = origem;
        assert(destino.posicao == 2 && destino.itens.size() == 3);
        assert(destino.next() == "c" && !destino.hasNext());
        assert(origem.posicao == 2 && origem.hasNext());
    }
};

int main() {
    std::cout << "===== TESTE UNITARIO: ITERADOR =====" << std::endl;
    TesteIterador::estadoInicial();
    TesteIterador::percorreTodosOsItens();
    TesteIterador::listaVazia();
    TesteIterador::guardaCopiaDaLista();
    TesteIterador::copiaPreservaPosicao();
    TesteIterador::atribuicao();
    std::cout << "  test_unit_iterador: OK" << std::endl;
    std::cout << "====================================" << std::endl;
    return 0;
}
