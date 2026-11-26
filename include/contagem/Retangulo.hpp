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
    /** @brief Cria uma marcação vazia: posição (0, 0) e tamanho 0 x 0. */
    Retangulo();

    /**
     * @brief Cria uma marcação.
     * @param x       Posição horizontal do canto superior esquerdo, em pixels.
     * @param y       Posição vertical do canto superior esquerdo, em pixels.
     * @param largura Largura da caixa, em pixels.
     * @param altura  Altura da caixa, em pixels.
     */
    Retangulo(int x, int y, int largura, int altura);

    /** @brief Construtor de cópia: copia posição e tamanho de outro Retangulo. */
    Retangulo(const Retangulo&) = default;

    /** @brief Atribuição por cópia: copia posição e tamanho de outro Retangulo. */
    Retangulo& operator=(const Retangulo&) = default;

    /** @brief Destrutor. */
    ~Retangulo() = default;

    /** @brief Devolve a posição horizontal do canto superior esquerdo. */
    int x() const;

    /** @brief Devolve a posição vertical do canto superior esquerdo. */
    int y() const;

    /** @brief Devolve a largura da caixa. */
    int width() const;

    /** @brief Devolve a altura da caixa. */
    int height() const;

private:
    friend class TesteRetangulo; ///< Acesso do teste unitário (test_unit_retangulo.cpp).

    int posX;    ///< Posição horizontal (pixels).
    int posY;    ///< Posição vertical (pixels).
    int largura; ///< Largura (pixels).
    int altura;  ///< Altura (pixels).
};

#endif
