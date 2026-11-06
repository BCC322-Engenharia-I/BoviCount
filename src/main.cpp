#include <iostream>
#include "Usuario.hpp"
#include "Sessao.hpp"
#include "ServicoPastos.hpp"

static const char* texto(bool v) { return v ? "true" : "false"; }

int main() {
    Usuario gestor(1, "Gestor", 1);
    Sessao sessaoGestor(gestor);
    ServicoPastos pastos(sessaoGestor);

    // 1) Cadastra o pasto "Pasto Fundo" (o nome nao pode se repetir na fazenda).
    DadosPasto dados("Pasto Fundo", "Divisa Norte", 200);
    Pasto fundo;
    bool ok = pastos.create(dados, fundo);
    Pasto repetido;
    bool dup = pastos.create(dados, repetido);
    std::cout << "cadastro: " << texto(ok) << " | nome repetido: " << texto(dup) << std::endl;

    // 2) Consulta a lista de pastos da fazenda.
    Colecao<Pasto> lista = pastos.query(FiltroPasto());
    for (Iterador<Pasto> it = lista.iterator(); it.hasNext(); ) {
        const Pasto& p = it.next();
        std::cout << p.name() << " - " << p.location()
                  << " - capacidade " << p.capacity() << std::endl;
    }

    // 3) Altera a capacidade do pasto.
    bool editou = pastos.update(fundo.id(), DadosPasto("Pasto Fundo", "Divisa Norte", 250));
    bool invalido = pastos.update(fundo.id(), DadosPasto("Pasto Fundo", "Divisa Norte", 0));
    std::cout << "edicao: " << texto(editou) << " | capacidade 0: " << texto(invalido) << std::endl;

    // 4) Remocao de pastos.
    Pasto brejo;
    pastos.create(DadosPasto("Pasto Brejo", "Rio Sul", 120), brejo);
    bool removido = pastos.remove(brejo.id());
    std::cout << "remove pasto sem contagens: " << texto(removido) << std::endl;

    return 0;
}