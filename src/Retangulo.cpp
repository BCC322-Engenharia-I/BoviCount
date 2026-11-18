#include "Retangulo.hpp"

Retangulo::Retangulo(int x, int y, int largura, int altura)
    : posX(x), posY(y), largura(largura), altura(altura) {}

int Retangulo::x() const { return posX; }

int Retangulo::y() const { return posY; }

int Retangulo::width() const { return largura; }

int Retangulo::height() const { return altura; }
