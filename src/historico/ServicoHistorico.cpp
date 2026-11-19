/**
 * @file ServicoHistorico.cpp
 * @brief Implementação da classe ServicoHistorico.
 *
 * Também contém, em um espaço de nomes anônimo (invisíveis fora deste arquivo),
 * as funções auxiliares de permissão (podeConsultar, podeSalvar) e de filtro (atende).
 * @see ServicoHistorico.hpp
 */

#include "historico/ServicoHistorico.hpp"
#include "pasto/RepositorioPastos.hpp"

namespace {
    const char* const PAPEL_GESTOR = "Gestor";
    const char* const PAPEL_FUNCIONARIO = "Funcionário";
    const char* const PAPEL_ADMINISTRADOR = "Administrador";

    // Texto vazio ou so com espacos conta como nao preenchido.
    bool vazio(const std::string& texto) {
        return texto.find_first_not_of(" \t\r\n") == std::string::npos;
    }

    // Gestor e Administrador consultam o historico e o erro.
    bool podeConsultar(const Usuario& u) {
        return u.role() == PAPEL_GESTOR || u.role() == PAPEL_ADMINISTRADOR;
    }

    // Qualquer papel conhecido pode salvar uma contagem.
    bool podeSalvar(const Usuario& u) {
        return podeConsultar(u) || u.role() == PAPEL_FUNCIONARIO;
    }

    // O periodo inclui o primeiro e o ultimo dia.
    bool atende(const Registro& r, const FiltroHistorico& filtro) {
        const Data dia = r.dateTime().date();
        if (dia < filtro.start() || filtro.end() < dia) {
            return false;
        }
        if (filtro.pastureId() != 0 && r.pastureId() != filtro.pastureId()) {
            return false;
        }
        if (filtro.operatorId() != 0 && r.operatorId() != filtro.operatorId()) {
            return false;
        }
        return true;
    }
}

ServicoHistorico::ServicoHistorico(const Sessao& sessao)
    : sessao(sessao), repositorio(RepositorioHistorico::instancia()) {}

bool ServicoHistorico::save(const Contagem& contagem, int idPasto,
                            int totalConfirmado, Registro& registro) {
    const Usuario& usuario = sessao.user();

    if (!podeSalvar(usuario)) {
        return false;
    }

    // a contagem precisa ter vindo de uma foto analisada (total 0 e valido)
    const std::string foto = contagem.image().path();
    if (vazio(foto)) {
        return false;
    }
    if (totalConfirmado < 0) {
        return false;
    }

    // o pasto precisa existir na fazenda do usuario
    RepositorioPastos& pastos = RepositorioPastos::instancia();
    Pasto pasto;
    if (!pastos.buscar(idPasto, pasto) || pasto.propertyId() != usuario.propertyId()) {
        return false;
    }

    // Tudo foi conferido; agora guarda o registro e marca o pasto como "com
    // contagens". Se a segunda etapa falhar, a primeira e desfeita.
    Registro novo;
    try {
        novo = repositorio.inserir(DataHora::agora(), usuario.id(), idPasto,
                                   contagem.total(), totalConfirmado, foto,
                                   usuario.propertyId());
        try {
            pastos.registrarContagem(idPasto);
        } catch (...) {
            repositorio.remover(novo.id());
            return false;
        }
    } catch (...) {
        return false;
    }

    registro = novo;
    return true;
}

std::vector<Registro> ServicoHistorico::filtrar(const FiltroHistorico& filtro) const {
    std::vector<Registro> resultado;
    std::vector<Registro> registros = repositorio.listar(sessao.user().propertyId());

    for (std::size_t i = 0; i < registros.size(); ++i) {
        if (atende(registros[i], filtro)) {
            resultado.push_back(registros[i]);
        }
    }
    return resultado;
}

Colecao<Registro> ServicoHistorico::query(const FiltroHistorico& filtro) {
    Colecao<Registro> resultado;

    if (!podeConsultar(sessao.user())) {
        return resultado;
    }

    std::vector<Registro> registros = filtrar(filtro);
    for (std::size_t i = 0; i < registros.size(); ++i) {
        resultado.add(registros[i]);
    }
    return resultado;
}

bool ServicoHistorico::errorRate(const FiltroHistorico& filtro, double& taxa) {
    if (!podeConsultar(sessao.user())) {
        return false;
    }

    std::vector<Registro> registros = filtrar(filtro);
    double soma = 0.0;
    int usados = 0;

    for (std::size_t i = 0; i < registros.size(); ++i) {
        const int confirmado = registros[i].confirmedTotal();
        if (confirmado == 0) {
            continue;  // sem base para a porcentagem: fica fora do calculo
        }
        double diferenca = static_cast<double>(registros[i].automaticTotal() - confirmado);
        if (diferenca < 0) {
            diferenca = -diferenca;
        }
        soma += diferenca / confirmado * 100.0;
        ++usados;
    }

    if (usados == 0) {
        return false;  // nada para medir: 'taxa' continua como estava
    }

    taxa = soma / usados;
    return true;
}
