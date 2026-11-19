#ifndef CONTAGEM_GADO_SERVICOS_H
#define CONTAGEM_GADO_SERVICOS_H

#include "usuario_contagem_gado.h"
#include "contagem_gado.h"

namespace contagem_gado {

class ServicoContagem {
private:
    Sessao sessao_;
    MotorVisao& motor_;

public:
    ServicoContagem(const Sessao& sessao, MotorVisao& motor);
    bool countByImage(const Imagem& imagem, Contagem& contagem);
};

} // namespace contagem_gado

#endif
