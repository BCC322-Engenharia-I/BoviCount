#ifndef COLECAO_H
#define COLECAO_H

#include <vector>
#include "core/Iterador.hpp"

// Lista de resultados de uma consulta (de pastos, de contagens salvas etc.).
class TesteColecao;

template <class T>
class Colecao {
private:
    friend class TesteColecao;
    std::vector<T> itens;

public:
    Colecao() : itens() {}

    // Copia os itens de outra lista.
    Colecao(const Colecao&) = default;
    Colecao& operator=(const Colecao&) = default;
    ~Colecao() = default;

    // Acrescenta um item a lista.
    void add(const T& item) { itens.push_back(item); }

    // Permite percorrer a lista.
    Iterador<T> iterator() const { return Iterador<T>(itens); }

    // Quantos itens ha (0 = lista vazia).
    int size() const { return static_cast<int>(itens.size()); }
};

#endif // COLECAO_H