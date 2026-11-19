/**
 * @file RepositorioHistorico.cpp
 * @brief Implementação da classe RepositorioHistorico.
 * @see RepositorioHistorico.hpp
 */

#include "historico/RepositorioHistorico.hpp"

RepositorioHistorico::RepositorioHistorico() : entradas(), proximoId(1) {}

RepositorioHistorico& RepositorioHistorico::instancia() {
    static RepositorioHistorico repositorio;
    return repositorio;
}

Registro RepositorioHistorico::inserir(const DataHora& dataHora, int idOperador, int idPasto,
                                       int totalAutomatico, int totalConfirmado,
                                       const std::string& caminhoImagem, int idPropriedade) {
    Entrada entrada;
    entrada.registro = Registro(proximoId, dataHora, idOperador, idPasto,
                                totalAutomatico, totalConfirmado, caminhoImagem);
    entrada.idPropriedade = idPropriedade;
    entradas.push_back(entrada);
    ++proximoId;
    return entrada.registro;
}

std::vector<Registro> RepositorioHistorico::listar(int idPropriedade) const {
    std::vector<Registro> resultado;
    for (std::size_t i = 0; i < entradas.size(); ++i) {
        if (entradas[i].idPropriedade == idPropriedade) {
            resultado.push_back(entradas[i].registro);
        }
    }
    return resultado;
}

bool RepositorioHistorico::remover(int idRegistro) {
    for (std::size_t i = 0; i < entradas.size(); ++i) {
        if (entradas[i].registro.id() == idRegistro) {
            entradas.erase(entradas.begin() + i);
            return true;
        }
    }
    return false;
}

void RepositorioHistorico::limpar() {
    entradas.clear();
    proximoId = 1;
}
