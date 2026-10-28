# mostra os comandos disponiveis
help:
	@echo "Comandos disponiveis:"
	@echo "  make compile             - compila a aplicacao (bin/app)"
	@echo "  make run                 - executa a aplicacao"
	@echo "  make compile-test-pasto  - compila os testes do pasto (bin/test_pasto)"
	@echo "  make run-test-pasto      - executa os testes do pasto"

# compila a aplicacao
compile:
	g++ -Iinclude src/main.cpp src/Pasto.cpp src/PastoPlano.cpp src/PastoMorro.cpp src/GestaoPastoImpl.cpp -o bin/app

# roda a aplicacao
run:
	./bin/app

# compila os testes do pasto
compile-test-pasto:
	g++ -Iinclude src/Pasto.cpp src/PastoPlano.cpp src/PastoMorro.cpp src/GestaoPastoImpl.cpp test/test_pasto.cpp -o bin/test_pasto

# roda os testes do pasto
run-test-pasto:
	./bin/test_pasto