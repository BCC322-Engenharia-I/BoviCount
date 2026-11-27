<h1 align="center"> 🐂 BoviCount </h1>

<p align="center">
  <i>Software desktop de gestão rural para inventário automatizado de gado via Inteligência Artificial.</i>
</p>

O **BoviCount** é um software desktop de gestão rural desenvolvido para automatizar o inventário de gado de maneira não-intrusiva, eliminando a necessidade do estressante e custoso manejo manual dos animais até o curral. Utilizando inteligência artificial e visão computacional por meio do motor OpenCV e de um modelo YOLO pré-treinado, o sistema processa imagens aéreas dos pastos para identificar, demarcar com caixas delimitadoras (*bounding boxes*) e contar automaticamente os animais. O objetivo central da aplicação é garantir a precisão da contagem, reduzir altos custos logísticos e preservar o bem-estar do rebanho, mitigando perdas financeiras decorrentes de furtos ou machucados na pecuária extensiva.

Operando de forma totalmente *offline* para atender à realidade de baixa conectividade nas zonas rurais, o BoviCount armazena seus registros localmente utilizando um banco de dados embutido SQLite. Além do robusto módulo de contagem por IA, a aplicação fornece um ambiente completo de gestão (CRUD) para o controle administrativo de funcionários, pastos e capacidades de lotes. Essa arquitetura permite cruzar os dados de quem operou o sistema com a data, o pasto de origem e a mídia processada, gerando um histórico seguro, exportável em CSV e auditável para os gestores da propriedade.

## Como compilar e executar

Requisitos: `g++` com suporte a C++17 e `make`.

Na raiz do projeto:

```bash
make compile   # compila a aplicação e gera bin/app
make run       # executa bin/app
```

O comando `make` sem argumentos mostra a lista de comandos disponíveis.

## Testes

```bash
make test-all
```

Compila e executa todos os testes: os unitários (um arquivo por classe), os de pasto, histórico e contagem, e os funcionais, que rodam o `bin/app` de verdade. Cada teste imprime `OK` ao passar; se algum falhar, o programa aborta e mostra a linha.

Também é possível rodar por partes:

```bash
make compile-test-unitarios && make run-test-unitarios   # todos os unitários
make compile-test-unit-data && make run-test-unit-data   # uma classe só (pasto, registro, retangulo, imagem, contagem, data, datahora, iterador, colecao, regra)
make compile-test-pasto     && make run-test-pasto
make compile-test-historico && make run-test-historico
make compile-test-contagem  && make run-test-contagem
make compile-test-funcional && make run-test-funcional
```

## Documentação

```bash
make docs
```

Gera a documentação Doxygen em HTML (requer o [Doxygen](https://www.doxygen.nl/) instalado). Abra `doc/html/index.html` no navegador.