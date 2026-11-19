#ifndef FILTRO_PASTO_H
#define FILTRO_PASTO_H

#include <string>

// Criterio para buscar pastos: pelo nome (vazio = todos os pastos).
class FiltroPasto {
private:
    std::string nome;

public:
    FiltroPasto(const std::string& nome = "") : nome(nome) {}

    std::string name() const { return nome; }
};

#endif // FILTRO_PASTO_H