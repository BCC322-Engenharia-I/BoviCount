/**
 * @file Imagem.hpp
 * @brief Declaração da classe Imagem.
 */
#ifndef IMAGEM_HPP
#define IMAGEM_HPP

#include <string>

/**
 * @class Imagem
 * @brief A foto aérea do rebanho.
 *
 * Guarda apenas o caminho do arquivo (jpeg, jpg ou png) que será usado na
 * contagem. A classe não abre nem confere o arquivo: a validação de existência,
 * formato e resolução é responsabilidade do ServicoContagem.
 *
 * Uma Imagem com caminho vazio indica "nenhuma foto"; é assim que o
 * ServicoHistorico::save reconhece uma Contagem que não veio de uma foto analisada.
 *
 * @see Contagem
 */
class Imagem {
public:
    /** @brief Cria uma imagem sem caminho (nenhuma foto). */
    Imagem();

    /**
     * @brief Cria uma imagem a partir do caminho do arquivo.
     * @param caminho Local do arquivo no computador; aceita jpeg, jpg e png.
     */
    explicit Imagem(const std::string& caminho);

    /** @brief Devolve o caminho do arquivo ("" se não houver foto). */
    std::string path() const;

private:
    std::string caminho; ///< Caminho do arquivo da foto.
};

#endif