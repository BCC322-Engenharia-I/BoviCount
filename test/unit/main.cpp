#include "unit_tests_contagem_gado.h"
#include <iostream>

int main() {
    testeImagem();
    testeRetangulo();
    testeColecao();
    testeContagem();
    testeTempoContagem();
    testeUsuario();
    testeSessao();
    testeServicoContagemValida();
    testeServicoContagemVazia();
    testeFormatoInvalido();
    testeArquivoInexistente();
    testePermissao();
    testeFalhaMotor();
    testeLimite4K();

    std::cout << "\n14 testes unitarios aprovados.\n";
    return 0;
}
