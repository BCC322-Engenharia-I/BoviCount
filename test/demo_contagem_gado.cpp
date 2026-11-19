/**
 * @file demo_contagem_gado.cpp
 * @brief Demonstra o acionamento do servico de contagem, sem deteccao real por IA.
 */
#include "contagem_gado/contagem_gado.h"
#include "contagem_gado/usuario_contagem_gado.h"
#include "contagem_gado/servico_contagem_gado.h"
#include <iostream>

// Motor demonstrativo: confirma a chamada mas nao identifica bovinos.
class MotorDemonstracao : public contagem_gado::MotorVisao {
public:
    bool detect(const contagem_gado::Imagem& imagem,
                contagem_gado::Contagem& resultado) override {
        contagem_gado::Colecao<contagem_gado::Retangulo> marcacoes;
        resultado = contagem_gado::Contagem(imagem, marcacoes, 0.0);
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: make run-contagem IMAGEM=caminho/para/imagem.png\n";
        return 1;
    }

    contagem_gado::Usuario usuario(1, "Gestor", 1);
    contagem_gado::Sessao sessao(usuario);
    MotorDemonstracao motor;
    contagem_gado::ServicoContagem servico(sessao, motor);
    contagem_gado::Contagem resultado;

    bool sucesso = servico.countByImage(contagem_gado::Imagem(argv[1]), resultado);
    std::cout << (sucesso ? "Imagem validada e motor acionado.\n"
                         : "Imagem rejeitada ou processamento falhou.\n");
    if (sucesso) {
        std::cout << "Total demonstrativo: " << resultado.total() << "\n";
    }
    std::cout << "ATENCAO: motor demonstrativo, sem deteccao real de bovinos.\n";
    return sucesso ? 0 : 1;
}
