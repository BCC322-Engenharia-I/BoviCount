
SRC = src/Usuario.cpp src/Sessao.cpp src/DadosPasto.cpp src/Pasto.cpp src/RepositorioPastos.cpp src/ServicoPastos.cpp

CONTAGEM_GADO = src/contagem_gado.cpp src/servico_contagem_gado.cpp src/usuario_contagem_gado.cpp

help:
	@echo "make compile                  - compila o BoviCount"
	@echo "make run                      - executa o BoviCount"
	@echo "make compile-test-pasto       - compila os testes de pastos"
	@echo "make run-test-pasto           - executa os testes de pastos"
	@echo "make compile-test-contagem    - compila os testes unitarios de contagem de gado"
	@echo "make run-test-contagem        - executa os testes unitarios de contagem de gado"
	@echo "make clean                    - apaga os executaveis"

compile:
	mkdir -p bin
	g++ -Iinclude src/main.cpp $(SRC) $(CONTAGEM_GADO) -o bin/app

run:
	./bin/app

compile-test-pasto:
	mkdir -p bin
	g++ -Iinclude $(SRC) test/test_pasto.cpp -o bin/test_pasto

run-test-pasto:
	./bin/test_pasto

compile-test-contagem:
	mkdir -p bin
	g++ -Iinclude $(CONTAGEM_GADO) test/unit/unit_tests_contagem_gado.cpp test/unit/main.cpp -o bin/test_contagem_gado

run-test-contagem:
	./bin/test_contagem_gado

clean:
	rm -f bin/app bin/test_pasto bin/test_contagem_gado
