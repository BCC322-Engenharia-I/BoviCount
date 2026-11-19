#ifndef IMAGEM_HPP
#define IMAGEM_HPP

#include <string>

// A foto aerea do rebanho.
class Imagem {
public:
    Imagem();
    explicit Imagem(const std::string& caminho);

    std::string path() const;

private:
    std::string caminho;
};

#endif
