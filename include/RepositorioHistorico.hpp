#ifndef REPOSITORIOHISTORICO_HPP
#define REPOSITORIOHISTORICO_HPP

#include <string>
#include <vector>
#include "DataHora.hpp"
#include "Registro.hpp"

// Guarda as contagens salvas (em memoria). Existe uma unica instancia, assim
// como o RepositorioPastos; o ServicoHistorico e o unico que a usa em producao.
class RepositorioHistorico {
public:
    static RepositorioHistorico& instancia();

    // Guarda um registro novo (o repositorio define o numero) e o devolve.
    Registro inserir(const DataHora& dataHora, int idOperador, int idPasto,
                     int totalAutomatico, int totalConfirmado,
                     const std::string& caminhoImagem, int idPropriedade);

    // Registros da fazenda, na ordem em que foram salvos.
    std::vector<Registro> listar(int idPropriedade) const;

    // Desfaz um salvamento (usado quando algo falha no meio).
    bool remover(int idRegistro);

    // Apaga tudo (usado pelos testes).
    void limpar();

private:
    struct Entrada {
        Registro registro;
        int idPropriedade;
    };

    RepositorioHistorico();
    RepositorioHistorico(const RepositorioHistorico&);             // nao copiavel
    RepositorioHistorico& operator=(const RepositorioHistorico&);  // nao copiavel

    std::vector<Entrada> entradas;
    int proximoId;
};

#endif
