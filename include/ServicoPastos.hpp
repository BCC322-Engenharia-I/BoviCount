#ifndef SERVICO_PASTOS_H
#define SERVICO_PASTOS_H

#include <vector>
#include "Sessao.hpp"
#include "Regra.hpp"
#include "Consulta.hpp"
#include "DadosPasto.hpp"
#include "FiltroPasto.hpp"
#include "Pasto.hpp"
#include "RepositorioPastos.hpp"

// Servico responsavel pelos pastos. Tambem e uma "Consulta":
// busca pastos com query(filtro).
class ServicoPastos : public Consulta<Pasto, FiltroPasto> {
private:
    Sessao sessao;
    RepositorioPastos& repositorio;
    // Ponteiros NAO proprietarios: a regra deve viver mais que o servico.
    std::vector<const Regra<DadosPasto>*> regras;

    // Conferencias comuns ao cadastro e a edicao (dados completos,
    // capacidade > 0, nome unico na fazenda e regras da fazenda).
    // 'idIgnorado' e o pasto que nao conta como nome repetido (edicao).
    bool dadosValidos(const DadosPasto& dados, int idIgnorado) const;

public:
    // Recebe a Sessao para saber quem esta usando o sistema.
    explicit ServicoPastos(const Sessao& sessao);

    // CADASTRAR um pasto. Devolve true se cadastrou, false se recusou.
    // O sistema: 1) so deixa o Gestor (ou Administrador) cadastrar;
    //            2) confere nome, localizacao e capacidade maior que zero;
    //            3) confere se ja existe pasto com o mesmo nome na fazenda;
    //            4) consulta as regras proprias da fazenda, se houver;
    //            5) cadastra e devolve o pasto criado em 'pasto'.
    // Se recusar, 'pasto' continua como estava.
    bool create(const DadosPasto& dados, Pasto& pasto);

    // CONSULTAR pastos. Mostra so os pastos da fazenda do usuario (Gestor,
    // Funcionario ou Administrador), filtrados pelo nome. Se nada for
    // encontrado, a lista vem vazia.
    Colecao<Pasto> query(const FiltroPasto& filtro);

    // EDITAR um pasto. Faz as mesmas conferencias do cadastro; o nome do
    // proprio pasto nao conta como repetido. Devolve false se o pasto nao
    // existe ou e de outra fazenda.
    bool update(int idPasto, const DadosPasto& dados);

    // REMOVER um pasto. Devolve false se o pasto nao existe, e de outra
    // fazenda ou ja tem contagens salvas (para preservar o historico).
    bool remove(int idPasto);

    // Liga ao cadastro uma regra propria da fazenda.
    void add(const Regra<DadosPasto>& regra);
};

#endif // SERVICO_PASTOS_H