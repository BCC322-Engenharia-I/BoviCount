#ifndef PASTO_H
#define PASTO_H

#include <string>

// Classe abstrata base para os tipos de pasto.
class Pasto {
protected:
    std::string nome;
    std::string lote;
    double area;

public:
    Pasto();
    Pasto(const std::string& nome, const std::string& lote, double area);
    Pasto(const Pasto& outro);
    Pasto& operator=(const Pasto& outro);
    virtual ~Pasto();

    std::string getNome() const;
    virtual int calcularCapacidade() const = 0;
};

#endif // PASTO_H