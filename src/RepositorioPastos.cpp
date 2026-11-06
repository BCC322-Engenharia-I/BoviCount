#include "RepositorioPastos.hpp"

RepositorioPastos::RepositorioPastos() : pastos(), pastosComContagem(), proximoId(1) {}

RepositorioPastos& RepositorioPastos::instancia() {
    static RepositorioPastos repositorio;
    return repositorio;
}

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

Pasto RepositorioPastos::inserir(const DadosPasto& dados, int idPropriedade) {
    Pasto novo(proximoId++, dados, idPropriedade);
    pastos.push_back(novo);
    return novo;
}

bool RepositorioPastos::buscar(int idPasto, Pasto& pasto) const {
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].id() == idPasto) {
            pasto = pastos[i];
            return true;
        }
    }
    return false;
}

std::vector<Pasto> RepositorioPastos::listar(int idPropriedade) const {
    std::vector<Pasto> resultado;
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].propertyId() == idPropriedade) {
            resultado.push_back(pastos[i]);
        }
    }
    return resultado;
}

bool RepositorioPastos::atualizar(int idPasto, const DadosPasto& dados) {
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].id() == idPasto) {
            pastos[i] = Pasto(idPasto, dados, pastos[i].propertyId());
            return true;
        }
    }
    return false;
}

bool RepositorioPastos::remover(int idPasto) {
    for (std::size_t i = 0; i < pastos.size(); ++i) {
        if (pastos[i].id() == idPasto) {
            pastos.erase(pastos.begin() + i);
            return true;
        }
    }
    return false;
}

void RepositorioPastos::registrarContagem(int idPasto) {
    pastosComContagem.insert(idPasto);
}

bool RepositorioPastos::temContagens(int idPasto) const {
    return pastosComContagem.count(idPasto) > 0;
}

void RepositorioPastos::limpar() {
    pastos.clear();
    pastosComContagem.clear();
    proximoId = 1;
}