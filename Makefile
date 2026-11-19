# compartilhado por todas as funcionalidades
SRC_BASE = src/usuario/Usuario.cpp src/usuario/Sessao.cpp

# gestao de pastos
SRC_PASTO = src/pasto/DadosPasto.cpp src/pasto/Pasto.cpp src/pasto/RepositorioPastos.cpp src/pasto/ServicoPastos.cpp

# historico de contagens 
SRC_HISTORICO = src/core/Data.cpp src/core/DataHora.cpp \
                src/contagem/Imagem.cpp src/contagem/Retangulo.cpp src/contagem/Contagem.cpp \
                src/historico/Registro.cpp src/historico/FiltroHistorico.cpp \
                src/historico/RepositorioHistorico.cpp src/historico/ServicoHistorico.cpp

# contagem de gado
SRC_CONTAGEM = src/contagem_gado/contagem_gado.cpp src/contagem_gado/servico_contagem_gado.cpp \
               src/contagem_gado/usuario_contagem_gado.cpp

# flags de compilacao
FLAGS = -Iinclude -std=c++17

# mostra os comandos disponiveis
help:
	@echo "Comandos disponiveis:"
	@echo "  make compile                          - compila a aplicacao"
	@echo "  make run                              - executa a aplicacao"
	@echo "  make compile-test-pasto               - compila os testes do pasto"
	@echo "  make run-test-pasto                   - executa os testes do pasto"
	@echo "  make compile-test-historico           - compila os testes do historico"
	@echo "  make run-test-historico               - executa os testes do historico"
	@echo "  make compile-test-contagem            - compila os testes unitarios da contagem"
	@echo "  make run-test-contagem                - executa os testes unitarios da contagem"
	@echo "  make test-all                         - compila e executa todos os testes"

# compila a aplicacao
compile:
	g++ $(FLAGS) src/main.cpp $(SRC_BASE) $(SRC_PASTO) $(SRC_HISTORICO) -o bin/app

# roda a aplicacao
run:
	./bin/app

# compila os testes do pasto
compile-test-pasto:
	g++ $(FLAGS) $(SRC_BASE) $(SRC_PASTO) test/test_pasto.cpp -o bin/test_pasto

# roda os testes do pasto
run-test-pasto:
	./bin/test_pasto

# compila os testes do historico
compile-test-historico:
	g++ $(FLAGS) $(SRC_BASE) $(SRC_PASTO) $(SRC_HISTORICO) test/test_historico.cpp -o bin/test_historico

# roda os testes do historico
run-test-historico:
	./bin/test_historico

# compila os testes unitarios da contagem de gado
compile-test-contagem:
	g++ $(FLAGS) $(SRC_CONTAGEM) test/test_contagem.cpp -o bin/test_contagem

# roda os testes unitarios da contagem de gado
run-test-contagem:
	./bin/test_contagem

# compila e roda todos os testes
test-all: compile-test-pasto compile-test-historico compile-test-contagem \
		  run-test-pasto run-test-historico run-test-contagem

# gera a documentacao Doxygen (precisa do doxygen instalado)
docs:
	doxygen Doxyfile