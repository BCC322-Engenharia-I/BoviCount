/**
 * @file FiltroPasto.hpp
 * @brief Declaração da classe FiltroPasto.
 */
#ifndef FILTRO_PASTO_H
#define FILTRO_PASTO_H

#include <string>

/**
 * @brief Critério para buscar pastos.
 *
 * Por enquanto o único critério é o nome. Um nome vazio significa
 * "todos os pastos".
 *
 * @see ServicoPastos::query
 */
class FiltroPasto {
private:
    std::string nome;   ///< Texto buscado no nome do pasto (vazio = todos).

public:
    /**
     * @brief Cria um filtro por nome.
     * @param nome Texto a procurar no nome do pasto. O padrão, vazio,
     *             não restringe a busca.
     */
    FiltroPasto(const std::string& nome = "") : nome(nome) {}

    /** @brief Construtor de cópia: copia o nome buscado de outro filtro. */
    FiltroPasto(const FiltroPasto&) = default;

    /** @brief Atribuição por cópia: copia o nome buscado de outro filtro. */
    FiltroPasto& operator=(const FiltroPasto&) = default;

    /** @brief Destrutor. */
    ~FiltroPasto() = default;

    /** @brief Texto buscado no nome do pasto (vazio = todos). */
    std::string name() const { return nome; }
};

#endif // FILTRO_PASTO_H