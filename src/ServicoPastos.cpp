#include "ServicoPastos.hpp"
#include <cctype>

namespace {
    const char* const PAPEL_GESTOR = "Gestor";
    const char* const PAPEL_FUNCIONARIO = "Funcionário";
    const char* const PAPEL_ADMINISTRADOR = "Administrador";

    // Texto vazio ou so com espacos conta como nao preenchido.
    bool vazio(const std::string& texto) {
        return texto.find_first_not_of(" \t\r\n") == std::string::npos;
    }

    std::string minusculo(const std::string& texto) {
        std::string r = texto;
        for (std::size_t i = 0; i < r.size(); ++i) {
            r[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(r[i])));
        }
        return r;
    }

    // Gestor e Administrador administram os pastos.
    bool podeAdministrar(const Usuario& u) {
        return u.role() == PAPEL_GESTOR || u.role() == PAPEL_ADMINISTRADOR;
    }

    // Qualquer papel conhecido pode consultar.
    bool podeConsultar(const Usuario& u) {
        return podeAdministrar(u) || u.role() == PAPEL_FUNCIONARIO;
    }
}

ServicoPastos::ServicoPastos(const Sessao& sessao)
    : sessao(sessao), repositorio(RepositorioPastos::instancia()), regras() {}

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

void ServicoPastos::add(const Regra<DadosPasto>& regra) {
    regras.push_back(&regra);
}