# compartilhado por todas as funcionalidades
SRC_BASE = src/usuario/Usuario.cpp src/usuario/Sessao.cpp

# modelo da contagem (Imagem, Retangulo, Contagem): usado por historico e contagem_gado
SRC_MODELO_CONTAGEM = src/contagem/Imagem.cpp src/contagem/Retangulo.cpp src/contagem/Contagem.cpp

# gestao de pastos
SRC_PASTO = src/pasto/DadosPasto.cpp src/pasto/Pasto.cpp src/pasto/RepositorioPastos.cpp src/pasto/ServicoPastos.cpp

# historico de contagens
SRC_HISTORICO = src/core/Data.cpp src/core/DataHora.cpp \
                $(SRC_MODELO_CONTAGEM) \
                src/historico/Registro.cpp src/historico/FiltroHistorico.cpp \
                src/historico/RepositorioHistorico.cpp src/historico/ServicoHistorico.cpp

# contagem de gado (servico + motor)
SRC_CONTAGEM = src/contagem_gado/servico_contagem_gado.cpp

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
	@echo "  make compile-test-funcional           - compila os testes funcionais"
	@echo "  make run-test-funcional               - executa os testes funcionais"
	@echo "  make test-all                         - compila e executa todos os testes"
	@echo "  make docs                              - gera a documentacao doxygen"

# compila a aplicacao
compile:
	g++ $(FLAGS) src/main.cpp $(SRC_BASE) $(SRC_PASTO) $(SRC_HISTORICO) $(SRC_CONTAGEM) -o bin/app

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

# compila os testes da contagem de gado
compile-test-contagem:
	g++ $(FLAGS) $(SRC_BASE) $(SRC_MODELO_CONTAGEM) $(SRC_CONTAGEM) test/test_contagem.cpp -o bin/test_contagem

# roda os testes da contagem de gado
run-test-contagem:
	./bin/test_contagem

# compila os testes funcionais (precisam do bin/app)
compile-test-funcional: compile
	g++ $(FLAGS) test/test_funcional.cpp -o bin/test_funcional

# roda os testes funcionais
run-test-funcional:
	./bin/test_funcional

# compila e roda todos os testes
test-all: compile-test-pasto compile-test-historico compile-test-contagem compile-test-funcional \
		  run-test-pasto run-test-historico run-test-contagem run-test-funcional

# gera a documentacao Doxygen (precisa do doxygen instalado)
docs:
	doxygen Doxyfile