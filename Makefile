SRC = src/Usuario.cpp src/Sessao.cpp src/DadosPasto.cpp src/Pasto.cpp src/RepositorioPastos.cpp src/ServicoPastos.cpp
CONTAGEM_GADO = src/contagem_gado/contagem_gado.cpp src/contagem_gado/servico_contagem_gado.cpp src/contagem_gado/usuario_contagem_gado.cpp

help:
	@echo "make compile                  - compila o BoviCount"
	@echo "make run                      - executa o BoviCount"
	@echo "make compile-test-pasto       - compila os testes de pastos"
	@echo "make run-test-pasto           - executa os testes de pastos"
	@echo "make compile-test-contagem    - compila os testes unitarios de contagem de gado"
	@echo "make run-test-contagem        - executa os testes unitarios de contagem de gado"

compile:
	mkdir -p bin
	g++ -std=c++17 -Iinclude src/main.cpp $(SRC) $(CONTAGEM_GADO) -o bin/app

run:
	./bin/app

compile-test-pasto:
	mkdir -p bin
	g++ -std=c++17 -Iinclude $(SRC) test/test_pasto.cpp -o bin/test_pasto

run-test-pasto:
	./bin/test_pasto

compile-test-contagem:
	mkdir -p bin
	g++ -std=c++17 $(CONTAGEM_GADO) test/unit/unit_tests_contagem_gado.cpp test/unit/main.cpp -o bin/test_contagem_gado

run-test-contagem:
	./bin/test_contagem_gado


clean:
	rm -f bin/app bin/test-pasto bin/test-contagem
