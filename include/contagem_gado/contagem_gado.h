/**
 * @file contagem_gado.h
 * @brief Contrato do motor de visão computacional usado na contagem de bovinos.
 *
 * Define apenas a interface MotorVisao. Os tipos Imagem, Retangulo e Contagem
 * são os do módulo `contagem/` (globais), compartilhados com o histórico.
 */

#ifndef CONTAGEM_GADO_CONTAGEM_H
#define CONTAGEM_GADO_CONTAGEM_H

#include "contagem/Contagem.hpp"
#include "contagem/Imagem.hpp"

namespace contagem_gado {

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
 *     bool detect(const Imagem& imagem, Contagem& saida) override;
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