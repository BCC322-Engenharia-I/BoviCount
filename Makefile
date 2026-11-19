SRC = src/Usuario.cpp src/Sessao.cpp src/DadosPasto.cpp src/Pasto.cpp src/RepositorioPastos.cpp src/ServicoPastos.cpp \
      src/Data.cpp src/DataHora.cpp src/Imagem.cpp src/Retangulo.cpp src/Contagem.cpp src/Registro.cpp \
      src/FiltroHistorico.cpp src/RepositorioHistorico.cpp src/ServicoHistorico.cpp

# mostra os comandos disponiveis
help:
	@echo "Comandos disponiveis:"
	@echo "  make compile                - compila a aplicacao (bin/app)"
	@echo "  make run                    - executa a aplicacao"
	@echo "  make compile-test-pasto     - compila os testes do pasto (bin/test_pasto)"
	@echo "  make run-test-pasto         - executa os testes do pasto"
	@echo "  make compile-test-historico - compila os testes do historico (bin/test_historico)"
	@echo "  make run-test-historico     - executa os testes do historico"
	@echo "  make compile-contagem       - compila apenas o modulo de contagem de gado"
	@echo "  make run-contagem IMAGEM=test/fundo-branco.jpeg - executa a demonstracao da contagem"

# compila a aplicacao
compile:
	g++ -Iinclude src/main.cpp $(SRC) -o bin/app

# roda a aplicacao
run:
	./bin/app

# compila os testes do pasto
compile-test-pasto:
	g++ -Iinclude $(SRC) test/test_pasto.cpp -o bin/test_pasto

# roda os testes do pasto
run-test-pasto:
	./bin/test_pasto

# compila os testes do historico
compile-test-historico:
	g++ -Iinclude $(SRC) test/test_historico.cpp -o bin/test_historico

# roda os testes do historico
run-test-historico:
	./bin/test_historico

# compila apenas o modulo de contagem de gado e seu programa de demonstracao
compile-contagem:
	mkdir -p bin
	g++ -std=c++17 -Iinclude src/contagem_gado.cpp src/servico_contagem_gado.cpp src/usuario_contagem_gado.cpp test/demo_contagem_gado.cpp -o bin/demo_contagem_gado

# executa somente a demonstracao de contagem de gado
run-contagem:
	./bin/demo_contagem_gado $(IMAGEM)
