#ifndef REPOSITORIO_PASTOS_H
#define REPOSITORIO_PASTOS_H

#include <set>
#include <string>
#include <vector>
#include "Pasto.hpp"

// Armazenamento dos pastos, compartilhado por todos os servicos.
// Por enquanto em memoria; a persistencia fica escondida atras desta classe.
class RepositorioPastos {
private:
    std::vector<Pasto> pastos;
    std::set<int> pastosComContagem;
    int proximoId;

public:
    RepositorioPastos();

    // Instancia unica usada pelos servicos.
    static RepositorioPastos& instancia();

    // Ja existe pasto com este nome na fazenda?
    // 'idIgnorado' permite desconsiderar um pasto (o proprio, na edicao).
    bool existeNome(int idPropriedade, const std::string& nome, int idIgnorado = 0) const;

    // Grava um novo pasto, gerando seu numero, e o devolve.
    Pasto inserir(const DadosPasto& dados, int idPropriedade);

    // Procura um pasto pelo numero. Devolve false se nao existe.
    bool buscar(int idPasto, Pasto& pasto) const;

    // Todos os pastos de uma fazenda.
    std::vector<Pasto> listar(int idPropriedade) const;

    // Troca os dados de um pasto. Devolve false se ele nao existe.
    bool atualizar(int idPasto, const DadosPasto& dados);

    // Apaga um pasto. Devolve false se ele nao existe.
    bool remover(int idPasto);

    // Contagens salvas ligadas ao pasto. O ServicoHistorico::save
    // (Inventario por IA) chamara registrarContagem ao salvar.
    void registrarContagem(int idPasto);
    bool temContagens(int idPasto) const;

    // Apaga tudo (usado nos testes).
    void limpar();
};

#endif // REPOSITORIO_PASTOS_H