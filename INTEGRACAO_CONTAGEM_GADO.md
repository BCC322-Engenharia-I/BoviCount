# Integracao do modulo contagem de gado

Este pacote parte do repositorio principal BoviCount e preserva o modulo de pastos.

- Codigo de contagem: `src/contagem_gado/` (namespace `contagem_gado`, para evitar conflito com Usuario, Sessao e Colecao existentes).
- Testes unitarios da contagem: `test/unit/unit_tests_contagem_gado.cpp` (14 testes).
- Uma das tres funcoes funcionais: `testeFuncionalContagemGado()` em `test/funcional/funcional_tests.cpp`.
- O `test/funcional/main.cpp` chama somente esta funcao, ate a integracao das duas funcoes dos colegas.

## Comandos

- `make compile && make run`: aplicativo principal, como antes.
- `make run-test-pasto`: testes ja existentes de pastos.
- `make run-test-contagem-gado`: 14 testes unitarios de contagem.
- `make run-funcional-contagem-gado`: um teste funcional de contagem.

**Limite:** a IA e simulada nos testes. O pacote nao implementa deteccao real YOLO/OpenCV nem valida a contagem real de animais presentes nas imagens.
