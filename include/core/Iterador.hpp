#ifndef ITERADOR_H
#define ITERADOR_H

#include <cstddef>
#include <vector>

// Percorre uma lista de resultados, um item de cada vez.
// Guarda uma copia dos itens, entao continua valido mesmo que a Colecao de origem deixe de existir.
class TesteIterador;

template <class T>
class Iterador {
private:
    friend class TesteIterador;
    std::vector<T> itens;
    std::size_t posicao;

public:
    // Sem lista nao ha o que percorrer.
    Iterador() = delete;

    explicit Iterador(const std::vector<T>& itens) : itens(itens), posicao(0) {}

    // Copia a lista e a posicao atual de outro iterador.
    Iterador(const Iterador&) = default;
    Iterador& operator=(const Iterador&) = default;
    ~Iterador() = default;

    // Ainda ha algum item para ver?
    bool hasNext() const { return posicao < itens.size(); }

    // Devolve o proximo item. Deve ser chamado somente se hasNext() for true.
    const T& next() { return itens[posicao++]; }
};

#endif // ITERADOR_H