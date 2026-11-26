/**
 * @file RepositorioPastos.hpp
 * @brief Declaração da classe RepositorioPastos.
 */
#ifndef REPOSITORIO_PASTOS_H
#define REPOSITORIO_PASTOS_H

#include <set>
#include <string>
#include <vector>
#include "pasto/Pasto.hpp"

/**
 * @brief Armazenamento dos pastos, compartilhado por todos os serviços.
 *
 * Por enquanto os dados ficam em memória; a persistência fica escondida
 * atrás desta classe. O repositório não verifica permissões nem regras de
 * negócio: isso é responsabilidade de ServicoPastos.
 *
 * @note Os pastos são mantidos em ordem de inserção e os identificadores
 *       são gerados em sequência, a partir de 1.
 *
 * @see ServicoPastos
 */
class RepositorioPastos {
private:
    std::vector<Pasto> pastos;          ///< Todos os pastos cadastrados, de todas as fazendas.
    std::set<int> pastosComContagem;    ///< Ids dos pastos que têm contagens salvas.
    int proximoId;                      ///< Próximo identificador a ser atribuído.

    /** @brief Atribuição proibida (declarado e não implementado). */
    RepositorioPastos& operator=(const RepositorioPastos&);
public:
    /** @brief Cria um repositório vazio, com o próximo id igual a 1. */
    RepositorioPastos();

    /**
     * @brief Instância única usada pelos serviços.
     * @return Referência à instância compartilhada (criada no primeiro uso).
     */
    static RepositorioPastos& instancia();

    /**
     * @brief Verifica se já existe pasto com este nome na fazenda.
     *
     * A comparação diferencia maiúsculas de minúsculas.
     *
     * @param idPropriedade Fazenda onde procurar.
     * @param nome          Nome a procurar.
     * @param idIgnorado    Pasto a desconsiderar (o próprio, na edição).
     *                      O padrão 0 não ignora nenhum.
     * @return @c true se existe outro pasto com o mesmo nome na fazenda.
     */
    bool existeNome(int idPropriedade, const std::string& nome, int idIgnorado = 0) const;

    /**
     * @brief Grava um novo pasto, gerando seu número.
     * @param dados         Dados do pasto.
     * @param idPropriedade Fazenda a que o pasto pertence.
     * @return O pasto gravado, já com o identificador gerado.
     */
    Pasto inserir(const DadosPasto& dados, int idPropriedade);

    /**
     * @brief Procura um pasto pelo número.
     * @param[in]  idPasto Número do pasto.
     * @param[out] pasto   Recebe o pasto encontrado; fica inalterado se não existir.
     * @return @c true se encontrou, @c false se o pasto não existe.
     */
    bool buscar(int idPasto, Pasto& pasto) const;

    /**
     * @brief Lista todos os pastos de uma fazenda.
     * @param idPropriedade Fazenda desejada.
     * @return Cópias dos pastos da fazenda, em ordem de inserção
     *         (vazio se não houver nenhum).
     */
    std::vector<Pasto> listar(int idPropriedade) const;

    /**
     * @brief Troca os dados de um pasto.
     *
     * O identificador e a fazenda do pasto são preservados.
     *
     * @param idPasto Número do pasto a alterar.
     * @param dados   Novos dados.
     * @return @c true se atualizou, @c false se o pasto não existe.
     */
    bool atualizar(int idPasto, const DadosPasto& dados);

    /**
     * @brief Apaga um pasto.
     * @param idPasto Número do pasto a apagar.
     * @return @c true se apagou, @c false se o pasto não existe.
     */
    bool remover(int idPasto);

    /**
     * @brief Registra que o pasto possui uma contagem salva.
     *
     * Será chamado por ServicoHistorico::save (Inventário por IA) ao salvar.
     * Um pasto com contagem registrada não pode ser removido.
     *
     * @param idPasto Número do pasto.
     */
    void registrarContagem(int idPasto);

    /**
     * @brief Informa se o pasto tem contagens salvas.
     * @param idPasto Número do pasto.
     * @return @c true se há ao menos uma contagem registrada.
     */
    bool temContagens(int idPasto) const;

    /**
     * @brief Apaga tudo e reinicia a numeração (usado nos testes).
     *
     * Remove todos os pastos e contagens e volta o próximo id para 1.
     */
    void limpar();
};

#endif // REPOSITORIO_PASTOS_H