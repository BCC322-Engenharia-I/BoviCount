/**
 * @file RepositorioPastos.cpp
 * @brief Implementação da classe RepositorioPastos.
 */
#include "pasto/RepositorioPastos.hpp"

/** @copydoc RepositorioPastos::RepositorioPastos() */
RepositorioPastos::RepositorioPastos() : pastos(), pastosComContagem(), proximoId(1) {}

/**
 * @copydoc RepositorioPastos::instancia()
 * @details Usa uma variável estática local (criada na primeira chamada).
 */
RepositorioPastos& RepositorioPastos::instancia() {
    static RepositorioPastos repositorio;
    return repositorio;
}

/**
 * @copydoc RepositorioPastos::existeNome()
 * @details Busca linear; considera apenas pastos da fazenda informada.
 */
bool RepositorioPastos::existeNome(int idPropriedade, const std::string& nome, int idIgnorado) const {
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].propertyId() == idPropriedade
            && pastos[i].id() != idIgnorado
            && pastos[i].name() == nome) {
            return true;
        }
    }
    return false;
}

/**
 * @copydoc RepositorioPastos::inserir()
 * @details O identificador é obtido de @c proximoId, que é incrementado.
 */
Pasto RepositorioPastos::inserir(const DadosPasto& dados, int idPropriedade) {
    Pasto novo(proximoId++, dados, idPropriedade);
    pastos.push_back(novo);
    return novo;
}

/** @copydoc RepositorioPastos::buscar() */
bool RepositorioPastos::buscar(int idPasto, Pasto& pasto) const {
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].id() == idPasto) {
            pasto = pastos[i];
            return true;
        }
    }
    return false;
}

/** @copydoc RepositorioPastos::listar() */
std::vector<Pasto> RepositorioPastos::listar(int idPropriedade) const {
    std::vector<Pasto> resultado;
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].propertyId() == idPropriedade) {
            resultado.push_back(pastos[i]);
        }
    }
    return resultado;
}

/**
 * @copydoc RepositorioPastos::atualizar()
 * @details Recria o Pasto mantendo o mesmo id e a mesma propriedade.
 */
bool RepositorioPastos::atualizar(int idPasto, const DadosPasto& dados) {
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].id() == idPasto) {
            pastos[i] = Pasto(idPasto, dados, pastos[i].propertyId());
            return true;
        }
    }
    return false;
}

/** @copydoc RepositorioPastos::remover() */
bool RepositorioPastos::remover(int idPasto) {
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].id() == idPasto) {
            pastos.erase(pastos.begin() + i);
            return true;
        }
    }
    return false;
}

/** @copydoc RepositorioPastos::registrarContagem() */
void RepositorioPastos::registrarContagem(int idPasto) {
    pastosComContagem.insert(idPasto);
}

/** @copydoc RepositorioPastos::temContagens() */
bool RepositorioPastos::temContagens(int idPasto) const {
    return pastosComContagem.count(idPasto) > 0;
}

/** @copydoc RepositorioPastos::limpar() */
void RepositorioPastos::limpar() {
    pastos.clear();
    pastosComContagem.clear();
    proximoId = 1;
}