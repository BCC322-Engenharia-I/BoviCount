/**
 * @file Retangulo.hpp
 * @brief Declaração da classe Retangulo.
 */

#ifndef RETANGULO_HPP
#define RETANGULO_HPP

/**
 * @class Retangulo
 * @brief A marcação desenhada ao redor de um animal na foto.
 *
 * Descreve uma caixa em pixels: posição do canto superior esquerdo, largura
 * e altura. Cada animal encontrado pelo MotorVisao gera um Retangulo.
 *
 * @see Contagem::boxes
 */
class Retangulo {
public:
    /**
     * @brief Cria uma marcação.
     * @param x       Posição horizontal do canto superior esquerdo, em pixels.
     * @param y       Posição vertical do canto superior esquerdo, em pixels.
     * @param largura Largura da caixa, em pixels.
     * @param altura  Altura da caixa, em pixels.
     */
    Retangulo(int x, int y, int largura, int altura);

    /** @brief Devolve a posição horizontal do canto superior esquerdo. */
    int x() const;

    /** @brief Devolve a posição vertical do canto superior esquerdo. */
    int y() const;

    /** @brief Devolve a largura da caixa. */
    int width() const;

    /** @brief Devolve a altura da caixa. */
    int height() const;

private:
    int posX;    ///< Posição horizontal (pixels).
    int posY;    ///< Posição vertical (pixels).
    int largura; ///< Largura (pixels).
    int altura;  ///< Altura (pixels).
};

#endif
