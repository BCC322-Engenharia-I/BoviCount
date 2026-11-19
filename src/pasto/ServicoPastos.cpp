/**
 * @file ServicoPastos.cpp
 * @brief Implementação da classe ServicoPastos.
 */
#include "pasto/ServicoPastos.hpp"
#include <cctype>

namespace {
    /** @brief Papel com permissão para administrar pastos. */
    const char* const PAPEL_GESTOR = "Gestor";
    /** @brief Papel que apenas consulta pastos. */
    const char* const PAPEL_FUNCIONARIO = "Funcionário";
    /** @brief Papel com permissão para administrar pastos. */
    const char* const PAPEL_ADMINISTRADOR = "Administrador";

    /**
     * @brief Verifica se um texto está "não preenchido".
     * @param texto Texto a verificar.
     * @return @c true se o texto é vazio ou contém só espaços, tabulações ou quebras de linha.
     */
    bool vazio(const std::string& texto) {
        return texto.find_first_not_of(" \t\r\n") == std::string::npos;
    }

    /**
     * @brief Converte um texto para minúsculas (caracteres ASCII).
     * @param texto Texto original.
     * @return Cópia do texto em minúsculas.
     */
    std::string minusculo(const std::string& texto) {
        std::string r = texto;
        for (std::size_t i = 0; i < r.size(); ++i) {
            r[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(r[i])));
        }
        return r;
    }

    /**
     * @brief Gestor e Administrador administram os pastos.
     * @param u Usuário a verificar.
     * @return @c true se o papel é Gestor ou Administrador.
     */
    bool podeAdministrar(const Usuario& u) {
        return u.role() == PAPEL_GESTOR || u.role() == PAPEL_ADMINISTRADOR;
    }

    /**
     * @brief Qualquer papel conhecido pode consultar.
     * @param u Usuário a verificar.
     * @return @c true se o papel é Gestor, Administrador ou Funcionário.
     */
    bool podeConsultar(const Usuario& u) {
        return podeAdministrar(u) || u.role() == PAPEL_FUNCIONARIO;
    }
}

/** @copydoc ServicoPastos::ServicoPastos(const Sessao&) */
ServicoPastos::ServicoPastos(const Sessao& sessao)
    : sessao(sessao), repositorio(RepositorioPastos::instancia()), regras() {}

/** @copydoc ServicoPastos::dadosValidos() */
bool ServicoPastos::dadosValidos(const DadosPasto& dados, int idIgnorado) const {
    // dados completos
    if (vazio(dados.name()) || vazio(dados.location()) || dados.capacity() <= 0) {
        return false;
    }

    // nome unico na fazenda
    if (repositorio.existeNome(sessao.user().propertyId(), dados.name(), idIgnorado)) {
        return false;
    }

    // regras proprias da fazenda
    for (std::size_t i = 0; i < regras.size(); ++i) {
        if (!regras[i]->allows(dados)) {
            return false;
        }
    }
    return true;
}

/** @copydoc ServicoPastos::create() */
bool ServicoPastos::create(const DadosPasto& dados, Pasto& pasto) {
    const Usuario& usuario = sessao.user();

    if (!podeAdministrar(usuario)) {
        return false;
    }
    if (!dadosValidos(dados, 0)) {
        return false;
    }

    pasto = repositorio.inserir(dados, usuario.propertyId());
    return true;
}

/** @copydoc ServicoPastos::query() */
Colecao<Pasto> ServicoPastos::query(const FiltroPasto& filtro) {
    Colecao<Pasto> resultado;
    const Usuario& usuario = sessao.user();

    if (!podeConsultar(usuario)) {
        return resultado;
    }

    const std::string busca = minusculo(filtro.name());
    std::vector<Pasto> pastos = repositorio.listar(usuario.propertyId());

    for (std::size_t i = 0; i < pastos.size(); ++i) {
        // Filtro vazio = todos; senao, o nome precisa conter o texto buscado.
        if (busca.empty() || minusculo(pastos[i].name()).find(busca) != std::string::npos) {
            resultado.add(pastos[i]);
        }
    }
    return resultado;
}

/** @copydoc ServicoPastos::update() */
bool ServicoPastos::update(int idPasto, const DadosPasto& dados) {
    const Usuario& usuario = sessao.user();

    if (!podeAdministrar(usuario)) {
        return false;
    }

    Pasto atual;
    if (!repositorio.buscar(idPasto, atual) || atual.propertyId() != usuario.propertyId()) {
        return false;
    }

    if (!dadosValidos(dados, idPasto)) {
        return false;
    }

    return repositorio.atualizar(idPasto, dados);
}

/** @copydoc ServicoPastos::remove() */
bool ServicoPastos::remove(int idPasto) {
    const Usuario& usuario = sessao.user();

    if (!podeAdministrar(usuario)) {
        return false;
    }

    Pasto atual;
    if (!repositorio.buscar(idPasto, atual) || atual.propertyId() != usuario.propertyId()) {
        return false;
    }

    // Preserva o historico: pasto com contagens salvas nao pode ser removido.
    if (repositorio.temContagens(idPasto)) {
        return false;
    }

    return repositorio.remover(idPasto);
}

/** @copydoc ServicoPastos::add() */
void ServicoPastos::add(const Regra<DadosPasto>& regra) {
    regras.push_back(&regra);
}