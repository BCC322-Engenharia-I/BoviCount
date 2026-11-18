#ifndef RETANGULO_HPP
#define RETANGULO_HPP

// A marcacao desenhada ao redor de um animal na foto.
class Retangulo {
public:
    Retangulo(int x, int y, int largura, int altura);

    int x() const;
    int y() const;
    int width() const;
    int height() const;

private:
    int posX;
    int posY;
    int largura;
    int altura;
};

#endif
