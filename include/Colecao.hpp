#ifndef COLECAO_H
#define COLECAO_H

#include <vector>
#include "Iterador.hpp"

// Lista de resultados de uma consulta (de pastos, de contagens salvas etc.).
template <class T>
class Colecao {
private:
    std::vector<T> itens;

public:
    Colecao() : itens() {}

    // Acrescenta um item a lista.
    void add(const T& item) { itens.push_back(item); }

    // Permite percorrer a lista.
    Iterador<T> iterator() const { return Iterador<T>(itens); }

    // Quantos itens ha (0 = lista vazia).
    int size() const { return static_cast<int>(itens.size()); }
};

#endif // COLECAO_H