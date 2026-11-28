#ifndef REGRA_H
#define REGRA_H

// Condicao propria de uma fazenda.
// Antes de cadastrar algo, o sistema pergunta a cada regra se aceita os dados.
class TesteRegra;

template <class T>
class Regra {
    friend class TesteRegra;
public:
    virtual ~Regra() {}

    // true = aceita; false = recusa
    virtual bool allows(const T& dados) const = 0;
};

#endif // REGRA_H