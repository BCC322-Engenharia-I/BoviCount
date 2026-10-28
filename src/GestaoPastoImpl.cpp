#include "GestaoPastoImpl.hpp"
#include <algorithm>
#include <stdexcept>

GestaoPastoImpl::GestaoPastoImpl() : pastos() {}

GestaoPastoImpl::GestaoPastoImpl(const GestaoPastoImpl& outro)
    : GestaoPasto(), pastos(outro.pastos) {}

GestaoPastoImpl& GestaoPastoImpl::operator=(const GestaoPastoImpl& outro) {
    if (this != &outro) {
        pastos = outro.pastos;
    }
    
    return *this;
}

GestaoPastoImpl::~GestaoPastoImpl() {}

void GestaoPastoImpl::cadastrarPasto(Pasto* p) {
    if (p == nullptr) {
        throw std::invalid_argument("cadastrarPasto: pasto nulo");
    }

    // Cadastro repetido do mesmo objeto é ignorado.
    if (std::find(pastos.begin(), pastos.end(), p) == pastos.end()) {
        pastos.push_back(p);
    }
}

void GestaoPastoImpl::removerPasto(Pasto* p) {
    pastos.erase(std::remove(pastos.begin(), pastos.end(), p), pastos.end());
}

std::vector<Pasto*> GestaoPastoImpl::listarPastos() const {
    return pastos;
}