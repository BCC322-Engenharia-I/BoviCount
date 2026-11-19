#ifndef CONTAGEM_GADO_USUARIO_H
#define CONTAGEM_GADO_USUARIO_H

#include <string>

namespace contagem_gado {

class Usuario {
private:
    int id_;
    std::string papel_;
    int idPropriedade_;

public:
    Usuario(int id, const std::string& papel, int idPropriedade);
    int id() const;
    std::string role() const;
    int propertyId() const;
};

class Sessao {
private:
    Usuario usuario_;

public:
    explicit Sessao(const Usuario& usuario);
    const Usuario& user() const;
};

} // namespace contagem_gado

#endif
