/**
 * @file ServicoPastos.hpp
 * @brief Declaração da classe ServicoPastos.
 */
#ifndef SERVICO_PASTOS_H
#define SERVICO_PASTOS_H

#include <vector>
#include "usuario/Sessao.hpp"
#include "core/Regra.hpp"
#include "core/Consulta.hpp"
#include "pasto/DadosPasto.hpp"
#include "pasto/FiltroPasto.hpp"
#include "pasto/Pasto.hpp"
#include "pasto/RepositorioPastos.hpp"

/**
 * @brief Serviço responsável pelos pastos (cadastrar, consultar, editar e remover).
 *
 * Também é uma Consulta: busca pastos com query(filtro).
 *
 * Permissões por papel do usuário da Sessao:
 * | Operação  | Gestor | Administrador | Funcionário |
 * |-----------|:------:|:-------------:|:-----------:|
 * | create    |   sim  |      sim      |     não     |
 * | query     |   sim  |      sim      |     sim     |
 * | update    |   sim  |      sim      |     não     |
 * | remove    |   sim  |      sim      |     não     |
 *
 * Todas as operações atuam apenas sobre os pastos da fazenda do usuário.
 *
 * @see Sessao
 * @see RepositorioPastos
 * @see Regra
 */
class ServicoPastos : public Consulta<Pasto, FiltroPasto> {
private:
    Sessao sessao;                      ///< Quem está usando o sistema.
    RepositorioPastos& repositorio;     ///< Repositório compartilhado (instância única).

    /**
     * @brief Regras próprias da fazenda.
     * @warning Ponteiros @b não proprietários: a regra deve viver mais que o serviço.
     */
    std::vector<const Regra<DadosPasto>*> regras;

    /**
     * @brief Conferências comuns ao cadastro e à edição.
     *
     * Verifica, nesta ordem: dados completos (nome e localização não vazios
     * e capacidade maior que zero), nome único na fazenda e regras próprias
     * da fazenda.
     *
     * @param dados      Dados a validar.
     * @param idIgnorado Pasto que não conta como nome repetido (edição);
     *                   use 0 no cadastro.
     * @return @c true se os dados são aceitos.
     */
    bool dadosValidos(const DadosPasto& dados, int idIgnorado) const;

public:
    /**
     * @brief Cria o serviço para a sessão informada.
     * @param sessao Sessão que identifica quem está usando o sistema.
     */
    explicit ServicoPastos(const Sessao& sessao);

    /** @brief Cópia proibida: o serviço está ligado ao repositório compartilhado, à sua sessão e a regras não proprietárias. */
    ServicoPastos(const ServicoPastos&) = delete;

    /** @brief Atribuição proibida: o serviço está ligado ao repositório compartilhado, à sua sessão e a regras não proprietárias. */
    ServicoPastos& operator=(const ServicoPastos&) = delete;

    /** @brief Destrutor. */
    ~ServicoPastos() = default;

    /**
     * @brief CADASTRAR um pasto.
     *
     * O sistema:
     *  -# só deixa o Gestor (ou Administrador) cadastrar;
     *  -# confere nome, localização e capacidade maior que zero;
     *  -# confere se já existe pasto com o mesmo nome na fazenda;
     *  -# consulta as regras próprias da fazenda, se houver;
     *  -# cadastra e devolve o pasto criado em @p pasto.
     *
     * @param[in]  dados Dados do novo pasto.
     * @param[out] pasto Recebe o pasto criado; se a operação for recusada, continua como estava.
     * @return @c true se cadastrou, @c false se recusou.
     */
    bool create(const DadosPasto& dados, Pasto& pasto);

    /**
     * @brief CONSULTAR pastos.
     *
     * Mostra só os pastos da fazenda do usuário (Gestor, Funcionário ou
     * Administrador), filtrados pelo nome. A busca é parcial e não diferencia
     * maiúsculas de minúsculas; filtro vazio devolve todos.
     *
     * @param filtro Critério de busca.
     * @return Coleção com os pastos encontrados; vazia se nada for encontrado
     *         ou se o papel do usuário não for reconhecido.
     */
    Colecao<Pasto> query(const FiltroPasto& filtro);

    /**
     * @brief EDITAR um pasto.
     *
     * Faz as mesmas conferências do cadastro; o nome do próprio pasto não
     * conta como repetido.
     *
     * @param idPasto Número do pasto a editar.
     * @param dados   Novos dados.
     * @return @c true se editou; @c false se o usuário não tem permissão, os
     *         dados são inválidos, ou o pasto não existe ou é de outra fazenda.
     */
    bool update(int idPasto, const DadosPasto& dados);

    /**
     * @brief REMOVER um pasto.
     *
     * @param idPasto Número do pasto a remover.
     * @return @c true se removeu; @c false se o usuário não tem permissão, o
     *         pasto não existe, é de outra fazenda ou já tem contagens salvas
     *         (para preservar o histórico).
     */
    bool remove(int idPasto);

    /**
     * @brief Liga ao cadastro uma regra própria da fazenda.
     *
     * A regra passa a valer no cadastro e na edição.
     *
     * @param regra Regra a aplicar. Não é copiada: deve viver mais que o serviço.
     */
    void add(const Regra<DadosPasto>& regra);
};

#endif // SERVICO_PASTOS_H