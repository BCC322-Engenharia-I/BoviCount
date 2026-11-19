/**
 * @file RepositorioHistorico.hpp
 * @brief Declaração da classe RepositorioHistorico.
 */

#ifndef REPOSITORIOHISTORICO_HPP
#define REPOSITORIOHISTORICO_HPP

#include <string>
#include <vector>
#include "core/DataHora.hpp"
#include "historico/Registro.hpp"

/**
 * @class RepositorioHistorico
 * @brief Guarda, em memória, as contagens salvas.
 *
 * Existe uma única instância (padrão Singleton), obtida com instancia(), assim
 * como o RepositorioPastos. Cada registro é guardado junto com o número da
 * fazenda a que pertence, de modo que cada fazenda enxerga só o que é seu.
 *
 * Em produção apenas o ServicoHistorico usa esta classe; os testes também a
 * usam para gravar registros com datas conhecidas e para limpar os dados.
 *
 * A classe não confere permissões nem valida dados: isso é trabalho do serviço.
 *
 * @note Não é copiável.
 * @see ServicoHistorico
 */
class RepositorioHistorico {
public:
    /**
     * @brief Devolve a única instância do repositório.
     * @return Referência ao repositório compartilhado.
     */
    static RepositorioHistorico& instancia();

    /**
     * @brief Guarda um registro novo e o devolve.
     *
     * O número do registro é definido pelo repositório (sequencial, a partir de 1).
     *
     * @param dataHora        Momento em que a contagem foi salva.
     * @param idOperador      Usuário que contou.
     * @param idPasto         Pasto contado.
     * @param totalAutomatico Total calculado pelo sistema.
     * @param totalConfirmado Total confirmado pelo usuário.
     * @param caminhoImagem   Foto com as marcações.
     * @param idPropriedade   Fazenda a que o registro pertence.
     * @return O registro guardado, já com o seu número.
     */
    Registro inserir(const DataHora& dataHora, int idOperador, int idPasto,
                     int totalAutomatico, int totalConfirmado,
                     const std::string& caminhoImagem, int idPropriedade);

    /**
     * @brief Lista os registros de uma fazenda.
     * @param idPropriedade Fazenda desejada.
     * @return Os registros dessa fazenda, na ordem em que foram salvos (vazio se não houver).
     */
    std::vector<Registro> listar(int idPropriedade) const;

    /**
     * @brief Remove um registro, desfazendo um salvamento.
     *
     * Usado pelo ServicoHistorico::save quando uma etapa falha depois de o
     * registro já ter sido guardado, para que nada fique salvo pela metade.
     *
     * @param idRegistro Número do registro a remover.
     * @return true se o registro existia e foi removido; false caso contrário.
     */
    bool remover(int idRegistro);

    /**
     * @brief Apaga todos os registros e reinicia a numeração.
     * @note Pensado para os testes.
     */
    void limpar();

private:
    /** @brief Um registro junto com a fazenda a que pertence. */
    struct Entrada {
        Registro registro;  ///< O registro guardado.
        int idPropriedade;  ///< Fazenda dona do registro.
    };

    /** @brief Construtor privado (Singleton). */
    RepositorioHistorico();

    /** @brief Cópia proibida (declarado e não implementado). */
    RepositorioHistorico(const RepositorioHistorico&);

    /** @brief Atribuição proibida (declarado e não implementado). */
    RepositorioHistorico& operator=(const RepositorioHistorico&);

    std::vector<Entrada> entradas; ///< Registros guardados.
    int proximoId;                 ///< Número do próximo registro.
};

#endif