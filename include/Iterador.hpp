#ifndef ITERADOR_H
#define ITERADOR_H

#include <cstddef>
#include <vector>

// Percorre uma lista de resultados, um item de cada vez.
// Guarda uma copia dos itens, entao continua valido mesmo que a Colecao de origem deixe de existir.
template <class T>
class Iterador {
private:
    std::vector<T> itens;
    std::size_t posicao;

public:
    explicit Iterador(const std::vector<T>& itens) : itens(itens), posicao(0) {}

    // Ainda ha algum item para ver?
    bool hasNext() const { return posicao < itens.size(); }

    // Devolve o proximo item. Deve ser chamado somente se hasNext() for true.
    const T& next() { return itens[posicao++]; }
};

#endif // ITERADOR_H