#ifndef CONTAGEM_GADO_CONTAGEM_H
#define CONTAGEM_GADO_CONTAGEM_H

#include <string>
#include <vector>

namespace contagem_gado {

// Colecao simples usada para armazenar as marcacoes feitas na foto.
template <class T>
class Colecao {
private:
    std::vector<T> itens_;

public:
    void add(const T& item) { itens_.push_back(item); }
    int size() const { return static_cast<int>(itens_.size()); }
};

class Imagem {
private:
    std::string caminho_;

public:
    Imagem();
    explicit Imagem(const std::string& caminho);
    std::string path() const;
};

class Retangulo {
private:
    int x_, y_, largura_, altura_;

public:
    Retangulo(int x, int y, int largura, int altura);
    int x() const;
    int y() const;
    int largura() const;
    int altura() const;
};

class Contagem {
private:
    Imagem imagem_;
    Colecao<Retangulo> caixas_;
    double segundos_;

public:
    Contagem();
    Contagem(const Imagem& imagem, const Colecao<Retangulo>& caixas,
             double segundos);
    int total() const;
    double processingSeconds() const;
    Colecao<Retangulo> boxes() const;
    Imagem image() const;
};

class MotorVisao {
public:
    virtual ~MotorVisao() = default;
    virtual bool detect(const Imagem& imagem, Contagem& contagem) = 0;
};

} // namespace contagem_gado

#endif
