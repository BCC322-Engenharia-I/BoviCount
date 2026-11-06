#ifndef CONSULTA_H
#define CONSULTA_H

#include "Colecao.hpp"

// Modelo comum a toda busca do sistema.
// Quem usa escreve sempre "query(filtro)", seja para buscar pastos ou contagens.
template <class T, class F>
class Consulta {
public:
    virtual ~Consulta() {}

    // Recebe um filtro (o que buscar) e devolve a lista de resultados.
    // A lista vem vazia quando nada e encontrado ou quando o usuario nao tem permissao.
    virtual Colecao<T> query(const F& filtro) = 0;
};

#endif // CONSULTA_H