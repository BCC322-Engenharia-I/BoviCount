#include <cassert>
#include <iostream>
#include "Usuario.hpp"
#include "Sessao.hpp"
#include "ServicoPastos.hpp"

class RegraCapacidadeMaxima : public Regra<DadosPasto> {
public:
    bool allows(const DadosPasto& d) const { return d.capacity() <= 500; }
};

static void limpar() {
    RepositorioPastos::instancia().limpar();
}

// ---------- Cadastrar ----------

// Cenario 1: Gestor cadastra "Pasto Fundo".
static void testeCadastroGestor() {
    limpar();
    ServicoPastos pastos(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo;
    assert(pastos.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));
    assert(fundo.id() != 0);
    assert(fundo.name() == "Pasto Fundo");
    assert(fundo.location() == "Divisa Norte");
    assert(fundo.capacity() == 200);
    assert(fundo.propertyId() == 1);

    Colecao<Pasto> lista = pastos.query(FiltroPasto());
    assert(lista.size() == 1);
}

static void testeCadastroAdministrador() {
    limpar();
    ServicoPastos pastos(Sessao(Usuario(2, "Administrador", 1)));

    Pasto p;
    assert(pastos.create(DadosPasto("Pasto A", "Sede", 50), p));
}

// Cenario 2: nome repetido e capacidade 0 sao recusados.
static void testeCadastroRecusas() {
    limpar();
    ServicoPastos pastos(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo;
    assert(pastos.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));

    Pasto repetido;
    assert(!pastos.create(DadosPasto("Pasto Fundo", "Outro Lugar", 100), repetido));
    assert(repetido.id() == 0);

    Pasto zero;
    assert(!pastos.create(DadosPasto("Pasto Zero", "Sede", 0), zero));
    assert(zero.id() == 0);

    Pasto negativo;
    assert(!pastos.create(DadosPasto("Pasto Neg", "Sede", -5), negativo));

    Pasto semNome;
    assert(!pastos.create(DadosPasto("   ", "Sede", 10), semNome));

    Pasto semLocal;
    assert(!pastos.create(DadosPasto("Pasto X", "", 10), semLocal));

    assert(pastos.query(FiltroPasto()).size() == 1);  // nada novo foi criado
}

// O nome so precisa ser unico dentro da fazenda.
static void testeNomeEmOutraFazenda() {
    limpar();
    ServicoPastos fazenda1(Sessao(Usuario(1, "Gestor", 1)));
    ServicoPastos fazenda2(Sessao(Usuario(9, "Gestor", 2)));

    Pasto a, b;
    assert(fazenda1.create(DadosPasto("Pasto Fundo", "Norte", 100), a));
    assert(fazenda2.create(DadosPasto("Pasto Fundo", "Sul", 100), b));
    assert(a.id() != b.id());
    assert(b.propertyId() == 2);
}

// Regra propria da fazenda (secao 2.4), valendo no cadastro e na edicao.
static void testeRegraDaFazenda() {
    limpar();
    ServicoPastos pastos(Sessao(Usuario(1, "Gestor", 1)));
    RegraCapacidadeMaxima regra;
    pastos.add(regra);

    Pasto grande;
    assert(!pastos.create(DadosPasto("Pasto Grande", "Sede", 501), grande));
    assert(grande.id() == 0);

    Pasto limite;
    assert(pastos.create(DadosPasto("Pasto Limite", "Sede", 500), limite));
    assert(!pastos.update(limite.id(), DadosPasto("Pasto Limite", "Sede", 501)));
}

// ---------- Consultar ----------

static void testeConsulta() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoPastos outraFazenda(Sessao(Usuario(9, "Gestor", 2)));

    Pasto a, b, c;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), a));
    assert(gestor.create(DadosPasto("Pasto Brejo", "Rio Sul", 120), b));
    assert(outraFazenda.create(DadosPasto("Pasto Alheio", "Longe", 80), c));

    // filtro vazio = todos os pastos da fazenda (nunca os de outra fazenda)
    assert(gestor.query(FiltroPasto()).size() == 2);
    assert(outraFazenda.query(FiltroPasto()).size() == 1);

    // busca pelo nome
    Colecao<Pasto> achados = gestor.query(FiltroPasto("Pasto Fundo"));
    assert(achados.size() == 1);
    Iterador<Pasto> it = achados.iterator();
    assert(it.hasNext());
    assert(it.next().id() == a.id());
    assert(!it.hasNext());

    // busca parcial e sem diferenciar maiusculas
    assert(gestor.query(FiltroPasto("brejo")).size() == 1);

    // nada encontrado = lista vazia
    assert(gestor.query(FiltroPasto("Inexistente")).size() == 0);
}

// Cenario 5: Funcionario nao cadastra, mas consulta normalmente.
static void testeFuncionario() {
    limpar();
    ServicoPastos gestor(Sessao(Usuario(1, "Gestor", 1)));
    ServicoPastos funcionario(Sessao(Usuario(5, "Funcionário", 1)));

    Pasto fundo;
    assert(gestor.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));

    Pasto novo;
    assert(!funcionario.create(DadosPasto("Pasto Novo", "Sede", 100), novo));
    assert(novo.id() == 0);

    assert(funcionario.query(FiltroPasto()).size() == 1);
    assert(funcionario.query(FiltroPasto("Pasto Fundo")).size() == 1);

    assert(!funcionario.update(fundo.id(), DadosPasto("Pasto Fundo", "Divisa Norte", 300)));
    assert(!funcionario.remove(fundo.id()));
    assert(gestor.query(FiltroPasto()).size() == 1);
}

// ---------- Editar ----------

// Cenarios 3 e do passo 5 da execucao: altera capacidade; capacidade 0 e recusada.
static void testeEdicao() {
    limpar();
    ServicoPastos pastos(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo, brejo;
    assert(pastos.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));
    assert(pastos.create(DadosPasto("Pasto Brejo", "Rio Sul", 120), brejo));

    // o nome do proprio pasto nao conta como repetido
    assert(pastos.update(fundo.id(), DadosPasto("Pasto Fundo", "Divisa Norte", 250)));
    Iterador<Pasto> it = pastos.query(FiltroPasto("Pasto Fundo")).iterator();
    assert(it.next().capacity() == 250);

    // capacidade 0 e recusada e o pasto continua com 250
    assert(!pastos.update(fundo.id(), DadosPasto("Pasto Fundo", "Divisa Norte", 0)));
    Iterador<Pasto> it2 = pastos.query(FiltroPasto("Pasto Fundo")).iterator();
    assert(it2.next().capacity() == 250);

    // nome de outro pasto da fazenda e recusado
    assert(!pastos.update(fundo.id(), DadosPasto("Pasto Brejo", "Divisa Norte", 250)));

    // campos vazios sao recusados
    assert(!pastos.update(fundo.id(), DadosPasto("", "Divisa Norte", 250)));
    assert(!pastos.update(fundo.id(), DadosPasto("Pasto Fundo", "", 250)));

    // pasto inexistente
    assert(!pastos.update(999, DadosPasto("Qualquer", "Lugar", 10)));
}

static void testeEdicaoOutraFazenda() {
    limpar();
    ServicoPastos fazenda1(Sessao(Usuario(1, "Gestor", 1)));
    ServicoPastos fazenda2(Sessao(Usuario(9, "Gestor", 2)));

    Pasto p;
    assert(fazenda1.create(DadosPasto("Pasto Fundo", "Norte", 200), p));
    assert(!fazenda2.update(p.id(), DadosPasto("Pasto Fundo", "Norte", 999)));
    assert(!fazenda2.remove(p.id()));
}

// ---------- Remover ----------

// Cenario 4: pasto com contagem salva nao pode ser removido.
static void testeRemocao() {
    limpar();
    ServicoPastos pastos(Sessao(Usuario(1, "Gestor", 1)));

    Pasto fundo, brejo;
    assert(pastos.create(DadosPasto("Pasto Fundo", "Divisa Norte", 200), fundo));
    assert(pastos.create(DadosPasto("Pasto Brejo", "Rio Sul", 120), brejo));

    // simula uma contagem salva no Pasto Fundo
    RepositorioPastos::instancia().registrarContagem(fundo.id());

    assert(!pastos.remove(fundo.id()));
    assert(pastos.query(FiltroPasto("Pasto Fundo")).size() == 1);

    // pasto sem contagens pode ser removido
    assert(pastos.remove(brejo.id()));
    assert(pastos.query(FiltroPasto("Pasto Brejo")).size() == 0);

    // pasto inexistente
    assert(!pastos.remove(999));
    assert(!pastos.remove(brejo.id()));
}

int main() {
    std::cout << "===== TESTE DE PASTO =====" << std::endl;
    testeCadastroGestor();
    testeCadastroAdministrador();
    testeCadastroRecusas();
    testeNomeEmOutraFazenda();
    testeRegraDaFazenda();
    testeConsulta();
    testeFuncionario();
    testeEdicao();
    testeEdicaoOutraFazenda();
    testeRemocao();
    std::cout << "  test_pasto: OK" << std::endl;
    std::cout << "==========================" << std::endl;
    return 0;
}