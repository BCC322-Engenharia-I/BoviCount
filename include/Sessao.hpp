#ifndef SESSAO_H
#define SESSAO_H

#include "Usuario.hpp"

// Guarda quem esta usando o sistema no momento.
// Todo servico consulta a Sessao para saber o que a pessoa pode fazer.
class Sessao {
private:
    Usuario usuario;

public:
    explicit Sessao(const Usuario& usuario);

    const Usuario& user() const;
};

#endif // SESSAO_H