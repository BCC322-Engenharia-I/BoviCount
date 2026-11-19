/**
 * @file contagem_gado.h
 * @brief Declaração das estruturas usadas na contagem de bovinos por imagem.
 *
 * Define a imagem de entrada, as marcações retangulares, o resultado da
 * contagem e o contrato do motor de visão. Os tipos pertencem ao namespace
 * contagem_gado para evitar conflitos com outros módulos do BoviCount.
 */

#ifndef CONTAGEM_GADO_CONTAGEM_H
#define CONTAGEM_GADO_CONTAGEM_H

#include <string>
#include <vector>

namespace contagem_gado {

// Colecao simples usada para armazenar as marcacoes feitas na foto.
/**
 * @class Colecao
 * @brief Coleção genérica, em memória, das marcações da imagem.
 *
 * Armazena elementos em std::vector; não realiza persistência nem valida
 * se os elementos representam bovinos distintos.
 * @tparam T Tipo dos elementos armazenados.
 */
template <class T>
class Colecao {
private:
    /** @brief Elementos armazenados na coleção. */
    std::vector<T> itens_;

public:
    /**
     * @brief Acrescenta um elemento à coleção.
     * @param item Elemento a ser armazenado.
     */
    void add(const T& item) { itens_.push_back(item); }
    /** @brief Devolve a quantidade de elementos armazenados. */
    int size() const { return static_cast<int>(itens_.size()); }
};

/**
 * @class Imagem
 * @brief Representa uma imagem por meio do caminho de seu arquivo.
 *
 * O construtor não verifica a existência ou o formato do arquivo; essas
 * validações são realizadas pelo ServicoContagem antes do processamento.
 * @see ServicoContagem
 */
class Imagem {
private:
    /** @brief Caminho do arquivo da imagem. */
    std::string caminho_;

public:
    /** @brief Cria uma imagem com caminho vazio. */
    Imagem();
    /** @brief Cria uma imagem com o caminho informado.
     * @param caminho Caminho do arquivo de imagem.
     */
    explicit Imagem(const std::string& caminho);
    /** @brief Devolve o caminho do arquivo. */
    std::string path() const;
};

/**
 * @class Retangulo
 * @brief Área retangular de marcação sobre uma imagem.
 *
 * Guarda coordenadas e dimensões informadas pelo motor de visão. Não
 * verifica, por si só, os limites da imagem ou a validade dos valores.
 */
class Retangulo {
private:
    /** @brief Posição horizontal, vertical, largura e altura da marcação. */
    int x_, y_, largura_, altura_;

public:
    /**
     * @brief Cria uma marcação com posição e dimensões informadas.
     * @param x Coordenada horizontal.
     * @param y Coordenada vertical.
     * @param largura Largura da marcação.
     * @param altura Altura da marcação.
     */
    Retangulo(int x, int y, int largura, int altura);
    /** @brief Devolve a coordenada horizontal da marcação. */
    int x() const;
    /** @brief Devolve a coordenada vertical da marcação. */
    int y() const;
    /** @brief Devolve a largura da marcação. */
    int largura() const;
    /** @brief Devolve a altura da marcação. */
    int altura() const;
};

/**
 * @class Contagem
 * @brief Resultado de uma análise de imagem.
 *
 * Reúne a imagem processada, as caixas delimitadoras e o tempo informado
 * pelo motor. O total é o número de caixas armazenadas, não uma contagem
 * validada por intervenção humana.
 * @see MotorVisao
 */
class Contagem {
private:
    /** @brief Imagem associada ao resultado. */
    Imagem imagem_;
    /** @brief Marcações retornadas pelo motor de visão. */
    Colecao<Retangulo> caixas_;
    /** @brief Tempo de processamento informado, em segundos. */
    double segundos_;

public:
    /** @brief Cria um resultado vazio, com tempo zero. */
    Contagem();
    /**
     * @brief Cria um resultado com imagem, marcações e duração.
     * @param imagem Imagem analisada.
     * @param caixas Caixas delimitadoras dos objetos detectados.
     * @param segundos Tempo de processamento em segundos.
     */
    Contagem(const Imagem& imagem, const Colecao<Retangulo>& caixas,
             double segundos);
    /** @brief Devolve a quantidade de caixas delimitadoras. */
    int total() const;
    /** @brief Devolve a tempo de processamento, em segundos. */
    double processingSeconds() const;
    /** @brief Devolve a cópia da coleção de caixas. */
    Colecao<Retangulo> boxes() const;
    /** @brief Devolve a cópia da imagem associada. */
    Imagem image() const;
};

/**
 * @class MotorVisao
 * @brief Interface abstrata para um mecanismo externo de detecção.
 *
 * Esta interface não implementa reconhecimento de animais. Uma classe
 * concreta precisa fornecer detect() para produzir uma Contagem.
 *
 * @par Exemplo
 * @code
 * class MotorConcreto : public contagem_gado::MotorVisao {
 * public:
 *     bool detect(const contagem_gado::Imagem& imagem,
 *                 contagem_gado::Contagem& saida) override;
 * };
 * @endcode
 * @see ServicoContagem
 */
class MotorVisao {
public:
    /** @brief Destrutor virtual para implementações do motor. */
    virtual ~MotorVisao() = default;
    /**
     * @brief Solicita a análise de uma imagem ao motor concreto.
     * @param imagem Imagem a ser analisada.
     * @param contagem Resultado a ser preenchido pela implementação.
     * @return true em caso de sucesso; false se a implementação falhar.
     */
    virtual bool detect(const Imagem& imagem, Contagem& contagem) = 0;
};

} // namespace contagem_gado

#endif
