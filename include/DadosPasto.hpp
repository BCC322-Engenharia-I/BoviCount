#ifndef DADOS_PASTO_H
#define DADOS_PASTO_H

#include <string>

// Dados que o Gestor informa ao cadastrar ou editar um pasto.
class DadosPasto {
private:
    std::string nome;
    std::string localizacao;
    int capacidade;

public:
    DadosPasto(const std::string& nome, const std::string& localizacao, int capacidade);

    std::string name() const;
    std::string location() const;
    int capacity() const;   // maximo de animais que o pasto comporta
};

#endif // DADOS_PASTO_H