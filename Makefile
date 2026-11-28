# compartilhado por todas as funcionalidades
SRC_BASE = src/usuario/Usuario.cpp src/usuario/Sessao.cpp

# modelo da contagem (Imagem, Retangulo, Contagem): usado por historico e contagem_gado
SRC_MODELO_CONTAGEM = src/contagem/Imagem.cpp src/contagem/Retangulo.cpp src/contagem/Contagem.cpp

# modelo dos pastos (sem o servico): usado por historico, que so precisa do repositorio
SRC_MODELO_PASTO = src/pasto/DadosPasto.cpp src/pasto/Pasto.cpp src/pasto/RepositorioPastos.cpp

# gestao de pastos
SRC_PASTO = $(SRC_MODELO_PASTO) src/pasto/ServicoPastos.cpp

# historico de contagens
SRC_HISTORICO = src/core/Data.cpp src/core/DataHora.cpp \
                $(SRC_MODELO_CONTAGEM) \
                src/historico/Registro.cpp src/historico/FiltroHistorico.cpp \
                src/historico/RepositorioHistorico.cpp src/historico/ServicoHistorico.cpp

# contagem de gado (servico + motor)
SRC_CONTAGEM = src/contagem_gado/servico_contagem_gado.cpp

# fontes minimas de cada teste unitario (cada um so linka a propria classe e o que ela contem)
SRC_UNIT_PASTO = src/pasto/DadosPasto.cpp src/pasto/Pasto.cpp
SRC_UNIT_REGISTRO = src/core/Data.cpp src/core/DataHora.cpp src/historico/Registro.cpp
SRC_UNIT_RETANGULO = src/contagem/Retangulo.cpp
SRC_UNIT_IMAGEM = src/contagem/Imagem.cpp
SRC_UNIT_CONTAGEM = $(SRC_MODELO_CONTAGEM)
SRC_UNIT_DATA = src/core/Data.cpp
SRC_UNIT_DATAHORA = src/core/Data.cpp src/core/DataHora.cpp

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
	@echo "  make compile-test-unitarios           - compila os testes unitarios (Pasto, Registro, Retangulo, Imagem, Contagem)"
	@echo "  make run-test-unitarios               - executa os testes unitarios"
	@echo "  make compile-test-unit-<classe>       - compila o teste unitario de uma classe (pasto, registro, retangulo, imagem, contagem)"
	@echo "  make run-test-unit-<classe>           - executa o teste unitario de uma classe"
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
	g++ $(FLAGS) $(SRC_BASE) $(SRC_MODELO_PASTO) $(SRC_HISTORICO) test/test_historico.cpp -o bin/test_historico

# roda os testes do historico
run-test-historico:
	./bin/test_historico

# compila os testes da contagem de gado
compile-test-contagem:
	g++ $(FLAGS) $(SRC_BASE) $(SRC_MODELO_CONTAGEM) $(SRC_CONTAGEM) test/test_contagem.cpp -o bin/test_contagem

# roda os testes da contagem de gado
run-test-contagem:
	./bin/test_contagem

# compila o teste unitario de Pasto
compile-test-unit-pasto:
	g++ $(FLAGS) $(SRC_UNIT_PASTO) test/test_unit_pasto.cpp -o bin/test_unit_pasto

# roda o teste unitario de Pasto
run-test-unit-pasto:
	./bin/test_unit_pasto

# compila o teste unitario de Registro
compile-test-unit-registro:
	g++ $(FLAGS) $(SRC_UNIT_REGISTRO) test/test_unit_registro.cpp -o bin/test_unit_registro

# roda o teste unitario de Registro
run-test-unit-registro:
	./bin/test_unit_registro

# compila o teste unitario de Retangulo
compile-test-unit-retangulo:
	g++ $(FLAGS) $(SRC_UNIT_RETANGULO) test/test_unit_retangulo.cpp -o bin/test_unit_retangulo

# roda o teste unitario de Retangulo
run-test-unit-retangulo:
	./bin/test_unit_retangulo

# compila o teste unitario de Imagem
compile-test-unit-imagem:
	g++ $(FLAGS) $(SRC_UNIT_IMAGEM) test/test_unit_imagem.cpp -o bin/test_unit_imagem

# roda o teste unitario de Imagem
run-test-unit-imagem:
	./bin/test_unit_imagem

# compila o teste unitario de Contagem
compile-test-unit-contagem:
	g++ $(FLAGS) $(SRC_UNIT_CONTAGEM) test/test_unit_contagem.cpp -o bin/test_unit_contagem

# roda o teste unitario de Contagem
run-test-unit-contagem:
	./bin/test_unit_contagem

# compila o teste unitario de Data
compile-test-unit-data:
	g++ $(FLAGS) $(SRC_UNIT_DATA) test/test_unit_data.cpp -o bin/test_unit_data

# roda o teste unitario de Data
run-test-unit-data:
	./bin/test_unit_data

# compila o teste unitario de DataHora
compile-test-unit-datahora:
	g++ $(FLAGS) $(SRC_UNIT_DATAHORA) test/test_unit_datahora.cpp -o bin/test_unit_datahora

# roda o teste unitario de DataHora
run-test-unit-datahora:
	./bin/test_unit_datahora

# compila o teste unitario de Iterador
compile-test-unit-iterador:
	g++ $(FLAGS) test/test_unit_iterador.cpp -o bin/test_unit_iterador

# roda o teste unitario de Iterador
run-test-unit-iterador:
	./bin/test_unit_iterador

# compila o teste unitario de Colecao
compile-test-unit-colecao:
	g++ $(FLAGS) test/test_unit_colecao.cpp -o bin/test_unit_colecao

# roda o teste unitario de Colecao
run-test-unit-colecao:
	./bin/test_unit_colecao

# compila o teste unitario de Regra
compile-test-unit-regra:
	g++ $(FLAGS) test/test_unit_regra.cpp -o bin/test_unit_regra

# roda o teste unitario de Regra
run-test-unit-regra:
	./bin/test_unit_regra

# compila e roda todos os testes unitarios (um arquivo por classe)
compile-test-unitarios: compile-test-unit-pasto compile-test-unit-registro compile-test-unit-retangulo \
						compile-test-unit-imagem compile-test-unit-contagem \
						compile-test-unit-data compile-test-unit-datahora compile-test-unit-iterador \
						compile-test-unit-colecao compile-test-unit-regra

run-test-unitarios: run-test-unit-pasto run-test-unit-registro run-test-unit-retangulo \
					run-test-unit-imagem run-test-unit-contagem \
					run-test-unit-data run-test-unit-datahora run-test-unit-iterador \
					run-test-unit-colecao run-test-unit-regra

# compila os testes funcionais (precisam do bin/app)
compile-test-funcional: compile
	g++ $(FLAGS) test/test_funcional.cpp -o bin/test_funcional

# roda os testes funcionais
run-test-funcional:
	./bin/test_funcional

# compila e roda todos os testes
test-all: compile-test-unitarios compile-test-pasto compile-test-historico compile-test-contagem compile-test-funcional \
		  run-test-unitarios run-test-pasto run-test-historico run-test-contagem run-test-funcional

# gera a documentacao Doxygen (precisa do doxygen instalado)
docs:
	doxygen Doxyfile