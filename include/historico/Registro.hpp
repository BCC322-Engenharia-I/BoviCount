/**
 * @file Registro.hpp
 * @brief Declaração da classe Registro.
 */

#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <string>
#include "core/DataHora.hpp"

/**
 * @class Registro
 * @brief Uma contagem salva no histórico.
 *
 * Guarda quem contou, quando, em qual pasto, o total calculado pelo sistema, o
 * total confirmado (ou corrigido) pelo usuário e a foto com as marcações. É
 * criado pelo RepositorioHistorico quando o ServicoHistorico::save aceita uma
 * contagem, e devolvido pelas consultas do ServicoHistorico.
 *
 * Um Registro padrão (id 0) significa "nenhum registro".
 *
 * @see ServicoHistorico
 * @see RepositorioHistorico
 */
class Registro {
public:
    /** @brief Cria um registro vazio (id 0, totais 0, sem foto). */
    Registro();

    /**
     * @brief Cria um registro completo.
     * @param id              Número que identifica o registro.
     * @param dataHora        Momento em que a contagem foi salva.
     * @param idOperador      Número do usuário que estava usando o sistema.
     * @param idPasto         Número do pasto em que a contagem foi feita.
     * @param totalAutomatico Total calculado pelo sistema.
     * @param totalConfirmado Total confirmado (ou corrigido) pelo usuário.
     * @param caminhoImagem   Caminho da foto com as marcações.
     */
    Registro(int id, const DataHora& dataHora, int idOperador, int idPasto,
             int totalAutomatico, int totalConfirmado, const std::string& caminhoImagem);

    /** @brief Construtor de cópia: copia todos os campos de outro Registro. */
    Registro(const Registro&) = default;

    /** @brief Atribuição por cópia: copia todos os campos de outro Registro. */
    Registro& operator=(const Registro&) = default;

    /** @brief Destrutor. */
    ~Registro() = default;

    /** @brief Devolve o número que identifica o registro (0 = registro vazio). */
    int id() const;

    /** @brief Devolve o momento em que a contagem foi salva. */
    DataHora dateTime() const;

    /** @brief Devolve o número do usuário que contou. */
    int operatorId() const;

    /** @brief Devolve o número do pasto em que a contagem foi feita. */
    int pastureId() const;

    /** @brief Devolve o total calculado pelo sistema. */
    int automaticTotal() const;

    /** @brief Devolve o total confirmado (ou corrigido) pelo usuário. */
    int confirmedTotal() const;

    /** @brief Devolve o caminho da foto com as marcações. */
    std::string imagePath() const;

private:
    friend class TesteRegistro; ///< Acesso do teste unitário (test_unit_registro.cpp).

    int idRegistro;            ///< Número do registro.
    DataHora dataHora;         ///< Momento em que foi salvo.
    int idOperador;            ///< Usuário que contou.
    int idPasto;               ///< Pasto contado.
    int totalAutomatico;       ///< Total calculado pelo sistema.
    int totalConfirmado;       ///< Total confirmado pelo usuário.
    std::string caminhoImagem; ///< Foto com as marcações.
};

#endif
