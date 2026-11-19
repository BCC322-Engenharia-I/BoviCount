/**
 * @file servico_contagem_gado.h
 * @brief Declaração do serviço de validação e execução de contagens.
 */

#ifndef CONTAGEM_GADO_SERVICOS_H
#define CONTAGEM_GADO_SERVICOS_H

#include "contagem_gado/usuario_contagem_gado.h"
#include "contagem_gado/contagem_gado.h"

namespace contagem_gado {

/**
 * @class ServicoContagem
 * @brief Valida pedidos de contagem e delega a análise ao motor de visão.
 *
 * O serviço verifica o papel do usuário, a existência do arquivo, o formato
 * JPG/JPEG/PNG e o limite de 4K UHD (3840 x 2160, em ambas as orientações).
 * Se todas as verificações forem aceitas, chama MotorVisao::detect().
 *
 * | Operação       | Gestor | Administrador | Funcionário |
 * |----------------|:------:|:-------------:|:-----------:|
 * | countByImage   |  sim   |      sim      |     sim     |
 *
 * Outros papéis são recusados. A contagem de saída só é substituída quando
 * o motor retorna true. O motor é fornecido pela aplicação; esta classe não
 * contém o algoritmo de detecção de bovinos.
 *
 * @par Exemplo
 * @code
 * // motor deve ser uma implementação concreta de MotorVisao.
 * contagem_gado::Usuario u(1, "Gestor", 10);
 * contagem_gado::Sessao sessao(u);
 * contagem_gado::ServicoContagem servico(sessao, motor);
 * contagem_gado::Contagem resultado;
 * bool ok = servico.countByImage(contagem_gado::Imagem("pasto.jpg"), resultado);
 * @endcode
 * @see MotorVisao
 * @see Sessao
 * @see Contagem
 */
class ServicoContagem {
private:
    /** @brief Cópia da sessão que solicita a contagem. */
    Sessao sessao_;
    /** @brief Motor de detecção fornecido pela aplicação; não é possuído pelo serviço. */
    MotorVisao& motor_;

public:
    /**
     * @brief Cria o serviço com a sessão e o motor de detecção.
     * @param sessao Sessão com o usuário que solicita o processamento.
     * @param motor Motor de visão a ser utilizado; deve permanecer válido
     *              durante a vida útil do serviço.
     */
    ServicoContagem(const Sessao& sessao, MotorVisao& motor);
    /**
     * @brief Valida uma imagem e solicita ao motor a sua análise.
     *
     * Recusa papéis diferentes de Gestor, Administrador e Funcionário;
     * arquivos inexistentes, formatos não aceitos, cabeçalhos inválidos
     * ou imagens acima do limite de 4K UHD. A contagem de saída só muda
     * se o motor concluir com sucesso.
     *
     * @param imagem Imagem a ser analisada.
     * @param contagem Recebe o resultado do motor quando o retorno é true.
     *                 Permanece inalterada quando o retorno é false.
     * @return true se todas as validações e a análise forem concluídas;
     *         false se alguma validação ou o motor falhar.
     */
    bool countByImage(const Imagem& imagem, Contagem& contagem);
};

} // namespace contagem_gado

#endif
