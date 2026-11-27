/**
 * @file test_funcional.cpp
 * @brief Testes funcionais (caixa-preta) do BoviCount.
 *
 * Cada teste executa o programa real (bin/app) alimentando o menu interativo
 * pela entrada padrao, como um usuario faria, e confere o que o sistema
 * mostra na tela. Os cenarios seguem o "Projeto da API (Sprint 2)":
 * secoes 3 (execucoes), 4.1 a 4.4 (casos de uso) e 1.1 (indicadores).
 *
 * Como cada execucao e um processo novo, o estado (pastos e historico) comeca
 * vazio em todo teste.
 *
 * Pre-requisito: bin/app compilado (`make compile`).
 * Compile com `make compile-test-funcional` e execute com `make run-test-funcional`.
 *
 * Nao alcancaveis pela interface (cobertos apenas nos testes unitarios):
 *  - Regra propria da fazenda (ServicoPastos::add);
 *  - Cenario 4 de 4.3 (queda de conexao durante o salvamento).
 */
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ---------- Verificacao ----------

/**
 * @brief Confere uma condição do teste; se falhar, mostra a linha, a saída do programa e
 *        encerra com erro.
 *
 * @param saida Texto que o programa mostrou, exibido junto da falha para ajudar a depurar.
 * @param cond  Condição que precisa ser verdadeira.
 */
#define EXIGE(saida, cond)                                                       \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::cerr << "FALHA: " << __FILE__ << ":" << __LINE__                \
                      << " -> " #cond << "\n----- saida do programa -----\n"     \
                      << (saida) << "\n-----------------------------\n";         \
            std::exit(1);                                                        \
        }                                                                        \
    } while (0)

/**
 * @brief Executa uma função de teste e imprime o nome dela seguido de OK quando termina sem
 *        falhar.
 *
 * @param teste Nome da função de teste, sem parênteses.
 */
#define RODAR(teste)                                                             \
    do {                                                                         \
        teste();                                                                 \
        std::cout << "  " #teste ": OK" << std::endl;                            \
    } while (0)

// ---------- Constantes da interface ----------

/** @brief Opção 1 da tela de login: papel Gestor. */
static const int GESTOR = 1;
/** @brief Opção 2 da tela de login: papel Funcionário. */
static const int FUNCIONARIO = 2;
/** @brief Opção 3 da tela de login: papel Administrador. */
static const int ADMINISTRADOR = 3;

/**
 * @brief Pergunta da busca por nome em Consultar pastos; a lista de pastos aparece logo depois
 *        dela.
 */
static const char* const PROMPT_BUSCA = "Buscar pelo nome (vazio = todos): ";
/**
 * @brief Última pergunta do filtro do histórico e do erro (id do funcionário); a resposta da
 *        consulta aparece logo depois dela.
 */
static const char* const PROMPT_FUNC = "Id do funcionario (0 = todos): ";
/** @brief Título do menu principal; marca onde termina a resposta de uma operação. */
static const char* const CABECALHO_MENU = "===== BOVICOUNT =====";

/**
 * @brief Período de datas digitado nos filtros do histórico e do erro (dia, mês e ano, inicial
 *        e final).
 */
struct Periodo {
    int di, mi, ai;  // dia, mes e ano iniciais
    int df, mf, af;  // dia, mes e ano finais
};
/**
 * @brief Período amplo (2000 a 2999) que inclui a data de hoje.
 *
 * O histórico carimba a data atual de cada contagem; por isso os testes usam um período amplo e
 * não dependem do dia em que rodam.
 */
static const Periodo TODO_PERIODO = {1, 1, 2000, 31, 12, 2999};
/**
 * @brief Período que não inclui a data de hoje (todo o ano de 2020), usado em consultas sem
 *        resultado.
 */
static const Periodo ANO_2020 = {1, 1, 2020, 31, 12, 2020};

// ---------- Arquivos de apoio ----------

/**
 * @brief Pasta temporária onde o teste cria as fotos, o roteiro de entrada e a saída do
 *        programa.
 *
 * @return Caminho da pasta temporária.
 */
static fs::path pasta() {
    return fs::temp_directory_path() / "bovicount_test_funcional";
}

/**
 * @brief Monta o caminho completo de um arquivo dentro da pasta temporária.
 *
 * @param nome Nome do arquivo.
 * @return Caminho completo do arquivo.
 */
static std::string foto(const std::string& nome) {
    return (pasta() / nome).string();
}

/**
 * @brief Grava bytes crus em um arquivo.
 *
 * @param arquivo Caminho do arquivo a criar.
 * @param b       Bytes a gravar.
 */
static void escreverBytes(const std::string& arquivo, const std::vector<unsigned char>& b) {
    std::ofstream saida(arquivo, std::ios::binary);
    saida.write(reinterpret_cast<const char*>(b.data()), static_cast<std::streamsize>(b.size()));
}

/**
 * @brief Cria um arquivo só com cabeçalho JPEG válido (SOI + SOF0) nas dimensões dadas.
 *
 * Serve para testar o formato e o limite de 4K sem precisar de uma foto real.
 *
 * @param arquivo Caminho do arquivo a criar.
 * @param largura Largura em pixels.
 * @param altura  Altura em pixels.
 */
static void criarJPEG(const std::string& arquivo, std::uint16_t largura, std::uint16_t altura) {
    std::vector<unsigned char> b = {0xFF, 0xD8, 0xFF, 0xC0, 0x00, 17, 8};
    b.push_back(static_cast<unsigned char>(altura >> 8));
    b.push_back(static_cast<unsigned char>(altura));
    b.push_back(static_cast<unsigned char>(largura >> 8));
    b.push_back(static_cast<unsigned char>(largura));
    const unsigned char resto[] = {3, 1, 0x22, 0, 2, 0x11, 1, 3, 0x11, 1};
    b.insert(b.end(), resto, resto + sizeof(resto));
    escreverBytes(arquivo, b);
}

/**
 * @brief Cria um arquivo com o texto dado, para simular formato errado ou cabeçalho inválido.
 *
 * @param arquivo Caminho do arquivo a criar.
 * @param texto   Conteúdo do arquivo.
 */
static void criarTexto(const std::string& arquivo, const std::string& texto) {
    std::ofstream saida(arquivo, std::ios::binary);
    saida << texto;
}

// ---------- Analise da saida ----------

/**
 * @brief Diz se um texto contém um trecho.
 *
 * @param s      Texto onde procurar.
 * @param trecho Trecho procurado.
 * @return @c true se o trecho aparece.
 */
static bool contem(const std::string& s, const std::string& trecho) {
    return s.find(trecho) != std::string::npos;
}

/**
 * @brief Conta quantas vezes um trecho aparece em um texto, sem sobreposição.
 *
 * @param s      Texto onde procurar.
 * @param trecho Trecho procurado.
 * @return Número de ocorrências.
 */
static int ocorrencias(const std::string& s, const std::string& trecho) {
    int n = 0;
    for (std::size_t p = s.find(trecho); p != std::string::npos; p = s.find(trecho, p + trecho.size())) {
        ++n;
    }
    return n;
}

/**
 * @brief Lista as respostas de sucesso e de recusa do sistema, na ordem em que apareceram.
 *
 * Cada resposta OK vira @c true e cada resposta RECUSADO vira @c false.
 *
 * @param s Saída completa do programa.
 * @return Sequência de respostas.
 */
static std::vector<bool> resultados(const std::string& s) {
    std::vector<bool> r;
    std::size_t p = 0;
    while (true) {
        const std::size_t ok = s.find(">> OK:", p);
        const std::size_t no = s.find(">> RECUSADO:", p);
        if (ok == std::string::npos && no == std::string::npos) break;
        if (no == std::string::npos || (ok != std::string::npos && ok < no)) {
            r.push_back(true);
            p = ok + 1;
        } else {
            r.push_back(false);
            p = no + 1;
        }
    }
    return r;
}

/**
 * @brief Isola a resposta de uma consulta: devolve o texto mostrado depois da n-ésima vez que o
 *        sistema fez a pergunta, até o próximo menu.
 *
 * Falha o teste se a pergunta não aparecer na saída.
 *
 * @param s      Saída completa do programa.
 * @param prompt Texto da pergunta.
 * @param indice Qual ocorrência da pergunta usar (0 é a primeira); -1 usa a última.
 * @return Trecho entre a pergunta e o próximo menu.
 */
static std::string aposPrompt(const std::string& s, const std::string& prompt, int indice = -1) {
    std::vector<std::size_t> pos;
    for (std::size_t p = s.find(prompt); p != std::string::npos; p = s.find(prompt, p + prompt.size())) {
        pos.push_back(p);
    }
    EXIGE(s, !pos.empty());
    const std::size_t i = indice < 0 ? pos.size() - 1 : static_cast<std::size_t>(indice);
    EXIGE(s, i < pos.size());
    const std::size_t ini = pos[i] + prompt.size();
    const std::size_t fim = s.find(CABECALHO_MENU, ini);
    return s.substr(ini, fim == std::string::npos ? std::string::npos : fim - ini);
}

/**
 * @brief Lê da saída o tempo de análise informado depois de uma contagem.
 *
 * @param s Saída completa do programa.
 * @return Tempo de análise em segundos.
 */
static double tempoDeAnalise(const std::string& s) {
    const std::string chave = "Tempo de analise: ";
    const std::size_t p = s.find(chave);
    EXIGE(s, p != std::string::npos);
    return std::stod(s.substr(p + chave.size()));
}

// ---------- Roteiro: o que o "usuario" digita ----------

/**
 * @brief Monta e executa a sequência de respostas que o usuário digitaria no menu do programa.
 *
 * Cada método corresponde a uma opção do menu e devolve o próprio Roteiro, para encadear as
 * chamadas. O método executar() roda o bin/app com essa entrada e devolve tudo o que ele
 * mostrou.
 */
class Roteiro {
private:
    /** @brief Respostas que serão enviadas ao programa, uma por linha. */
    std::vector<std::string> linhas;

public:
    /**
     * @brief Acrescenta uma resposta de texto.
     *
     * @param texto Texto digitado.
     * @return O próprio roteiro.
     */
    Roteiro& linha(const std::string& texto) { linhas.push_back(texto); return *this; }
    /**
     * @brief Acrescenta uma resposta numérica.
     *
     * @param numero Número digitado.
     * @return O próprio roteiro.
     */
    Roteiro& linha(int numero) { return linha(std::to_string(numero)); }

    /**
     * @brief Tela de login: papel, id do usuário e id da fazenda.
     *
     * @param papel   Opção do papel (GESTOR, FUNCIONARIO ou ADMINISTRADOR).
     * @param id      Id do usuário.
     * @param fazenda Id da fazenda.
     * @return O próprio roteiro.
     */
    Roteiro& login(int papel, int id, int fazenda) {
        return linha(papel).linha(id).linha(fazenda);
    }
    /**
     * @brief Menu 1: trocar de usuário.
     *
     * @param papel   Opção do novo papel.
     * @param id      Id do novo usuário.
     * @param fazenda Id da fazenda.
     * @return O próprio roteiro.
     */
    Roteiro& trocarUsuario(int papel, int id, int fazenda) {
        return linha(1).login(papel, id, fazenda);
    }
    /**
     * @brief Menu 2: cadastrar pasto.
     *
     * @param nome       Nome do pasto.
     * @param local      Localização do pasto.
     * @param capacidade Capacidade do pasto.
     * @return O próprio roteiro.
     */
    Roteiro& cadastrarPasto(const std::string& nome, const std::string& local, int capacidade) {
        return linha(2).linha(nome).linha(local).linha(capacidade);
    }
    /**
     * @brief Menu 3: consultar pastos.
     *
     * @param busca Trecho do nome a procurar; vazio mostra todos.
     * @return O próprio roteiro.
     */
    Roteiro& consultarPastos(const std::string& busca = "") {
        return linha(3).linha(busca);
    }
    /**
     * @brief Menu 4: editar pasto.
     *
     * @param id         Id do pasto a editar.
     * @param nome       Novo nome.
     * @param local      Nova localização.
     * @param capacidade Nova capacidade.
     * @return O próprio roteiro.
     */
    Roteiro& editarPasto(int id, const std::string& nome, const std::string& local, int capacidade) {
        return linha(4).linha(id).linha(nome).linha(local).linha(capacidade);
    }
    /**
     * @brief Menu 5: remover pasto.
     *
     * @param id Id do pasto a remover.
     * @return O próprio roteiro.
     */
    Roteiro& removerPasto(int id) {
        return linha(5).linha(id);
    }
    /**
     * @brief Menu 6: criar uma imagem de teste (PNG com cabeçalho válido).
     *
     * @param arquivo Caminho do arquivo a criar.
     * @param largura Largura em pixels.
     * @param altura  Altura em pixels.
     * @return O próprio roteiro.
     */
    Roteiro& criarImagem(const std::string& arquivo, int largura, int altura) {
        return linha(6).linha(arquivo).linha(largura).linha(altura);
    }
    /**
     * @brief Menu 7: contar o gado por foto; o motor simulado detecta o número de animais
     *        informado.
     *
     * @param caminho Caminho da foto.
     * @param animais Quantos animais o motor simulado deve detectar.
     * @return O próprio roteiro.
     */
    Roteiro& contar(const std::string& caminho, int animais) {
        return linha(7).linha(caminho).linha(animais);
    }
    /**
     * @brief Menu 8: salvar a última contagem.
     *
     * @param idPasto    Id do pasto contado.
     * @param confirmado Total confirmado pelo usuário.
     * @param confirma   @c true confirma o salvamento; @c false cancela.
     * @return O próprio roteiro.
     */
    Roteiro& salvar(int idPasto, int confirmado, bool confirma = true) {
        return linha(8).linha(idPasto).linha(confirmado).linha(confirma ? "s" : "n");
    }
    /**
     * @brief Menu 8 sem ter contado antes: o sistema avisa e não pergunta mais nada.
     *
     * @return O próprio roteiro.
     */
    Roteiro& salvarSemContagem() { return linha(8); }
    /**
     * @brief Conta uma foto nova (PNG 1920x1080) e salva a contagem.
     *
     * @param nomeFoto   Nome do arquivo da foto na pasta temporária.
     * @param animais    Quantos animais o motor simulado detecta.
     * @param idPasto    Id do pasto contado.
     * @param confirmado Total confirmado pelo usuário.
     * @return O próprio roteiro.
     */
    Roteiro& contagemSalva(const std::string& nomeFoto, int animais, int idPasto, int confirmado) {
        return criarImagem(foto(nomeFoto), 1920, 1080)
              .contar(foto(nomeFoto), animais)
              .salvar(idPasto, confirmado);
    }
    /**
     * @brief Menu 9: consultar o histórico.
     *
     * @param p          Período da consulta.
     * @param idPasto    Id do pasto; 0 mostra todos.
     * @param idOperador Id do funcionário; 0 mostra todos.
     * @return O próprio roteiro.
     */
    Roteiro& historico(const Periodo& p, int idPasto = 0, int idOperador = 0) {
        return linha(9).filtro(p, idPasto, idOperador);
    }
    /**
     * @brief Menu 10: erro da contagem.
     *
     * @param p          Período do cálculo.
     * @param idPasto    Id do pasto; 0 considera todos.
     * @param idOperador Id do funcionário; 0 considera todos.
     * @return O próprio roteiro.
     */
    Roteiro& erro(const Periodo& p, int idPasto = 0, int idOperador = 0) {
        return linha(10).filtro(p, idPasto, idOperador);
    }

    /**
     * @brief Executa o programa com o roteiro e devolve tudo o que ele mostrou.
     *
     * Sempre acrescenta a opção 0 no fim, para encerrar o programa. Falha o teste se o programa não
     * terminar normalmente: isso indica que o roteiro saiu de sincronia com o menu.
     *
     * @return Saída completa do programa.
     */
    std::string executar() {
        linhas.push_back("0");  // sempre encerra o programa no fim

        const fs::path entrada = pasta() / "entrada.txt";
        const fs::path saida = pasta() / "saida.txt";
        {
            std::ofstream out(entrada);
            for (std::size_t i = 0; i < linhas.size(); ++i) out << linhas[i] << "\n";
        }
        const std::string comando =
            "./bin/app < \"" + entrada.string() + "\" > \"" + saida.string() + "\" 2>&1";
        const int codigo = std::system(comando.c_str());

        std::ifstream in(saida);
        std::stringstream ss;
        ss << in.rdbuf();
        const std::string texto = ss.str();

        // Se o programa nao chegou ao "Ate logo!", o roteiro saiu de sincronia com o menu.
        EXIGE(texto, codigo == 0);
        EXIGE(texto, contem(texto, "Ate logo!"));
        EXIGE(texto, !contem(texto, "Entrada encerrada."));
        return texto;
    }

private:
    /**
     * @brief Digita o filtro de período, pasto e funcionário usado pelo histórico e pelo erro.
     *
     * @param p          Período.
     * @param idPasto    Id do pasto; 0 para todos.
     * @param idOperador Id do funcionário; 0 para todos.
     * @return O próprio roteiro.
     */
    Roteiro& filtro(const Periodo& p, int idPasto, int idOperador) {
        return linha(p.di).linha(p.mi).linha(p.ai)
              .linha(p.df).linha(p.mf).linha(p.af)
              .linha(idPasto).linha(idOperador);
    }
};

/**
 * @brief Roteiro inicial: Gestor (id 1, fazenda 1) com o Pasto Fundo (id 1) já cadastrado.
 *
 * @return Roteiro pronto para receber as próximas ações.
 */
static Roteiro comPastoFundo() {
    Roteiro r;
    r.login(GESTOR, 1, 1).cadastrarPasto("Pasto Fundo", "Divisa Norte", 200);
    return r;
}

/** @brief Sequência esperada de respostas: @c true para OK e @c false para RECUSADO. */
typedef std::vector<bool> Resp;

// =====================================================================
// RF-00  Acesso ao sistema e navegacao (login, troca de usuario, menu)
// =====================================================================

static void teste_acesso_login_e_troca_de_usuario() {
    Roteiro r;
    r.linha(7).linha(0)                 // opcoes de papel invalidas: o sistema pergunta de novo
     .linha(FUNCIONARIO).linha(5).linha(1)
     .trocarUsuario(GESTOR, 1, 1);
    const std::string s = r.executar();

    EXIGE(s, contem(s, "Logado como Funcionário (id 5, fazenda 1)"));
    EXIGE(s, contem(s, "Usuario: Funcionário (id 5, fazenda 1)"));
    EXIGE(s, contem(s, "Logado como Gestor (id 1, fazenda 1)"));
    EXIGE(s, ocorrencias(s, "Escolha o papel: ") == 4);
}

static void teste_acesso_opcao_invalida_do_menu() {
    Roteiro r;
    r.login(GESTOR, 1, 1).linha(99);
    const std::string s = r.executar();
    EXIGE(s, contem(s, "Opcao invalida."));
}

// =====================================================================
// RF-01  Cadastrar pasto/lote  (4.1)
// =====================================================================

// Cenario 1
static void teste_cadastrar_pasto_principal() {
    Roteiro r = comPastoFundo();
    r.consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true}));
    const std::string lista = aposPrompt(s, PROMPT_BUSCA);
    EXIGE(s, contem(lista, "[1] Pasto Fundo | Divisa Norte | capacidade 200"));
}

// Cenario 2: nome repetido, capacidade invalida e campos vazios
static void teste_cadastrar_pasto_recusas() {
    Roteiro r = comPastoFundo();
    r.cadastrarPasto("Pasto Fundo", "Outro Lugar", 100)  // nome repetido
     .cadastrarPasto("Pasto Zero", "Sede", 0)            // capacidade 0
     .cadastrarPasto("Pasto Neg", "Sede", -5)            // capacidade negativa
     .cadastrarPasto("Pasto Texto", "Sede", 0)           // texto nao numerico viraria 0
     .cadastrarPasto("   ", "Sede", 10)                  // nome so com espacos
     .cadastrarPasto("Pasto X", "", 10)                  // localizacao vazia
     .consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, false, false, false, false, false, false}));
    const std::string lista = aposPrompt(s, PROMPT_BUSCA);
    EXIGE(s, ocorrencias(lista, " | capacidade ") == 1);  // nada novo foi criado
}

// O nome so precisa ser unico dentro da fazenda
static void teste_cadastrar_pasto_nome_em_outra_fazenda() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(GESTOR, 9, 2)
     .cadastrarPasto("Pasto Fundo", "Sul", 100)
     .consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true}));
    const std::string lista = aposPrompt(s, PROMPT_BUSCA);
    EXIGE(s, ocorrencias(lista, " | capacidade ") == 1);
    EXIGE(s, contem(lista, "Pasto Fundo | Sul | capacidade 100"));
    EXIGE(s, !contem(lista, "Divisa Norte"));
}

// Cenario 5: Funcionario nao cadastra, mas consulta normalmente
static void teste_cadastrar_pasto_funcionario_recusado() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(FUNCIONARIO, 5, 1)
     .cadastrarPasto("Pasto Novo", "Sede", 100)
     .consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, false}));
    const std::string lista = aposPrompt(s, PROMPT_BUSCA);
    EXIGE(s, ocorrencias(lista, " | capacidade ") == 1);
    EXIGE(s, contem(lista, "Pasto Fundo"));
}

// O Administrador faz o mesmo que o Gestor
static void teste_cadastrar_pasto_administrador() {
    Roteiro r;
    r.login(ADMINISTRADOR, 2, 1).cadastrarPasto("Pasto A", "Sede", 50).consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true}));
    EXIGE(s, contem(aposPrompt(s, PROMPT_BUSCA), "Pasto A | Sede | capacidade 50"));
}

// =====================================================================
// RF-02  Consultar pastos/lotes
// =====================================================================

static void teste_consultar_pastos_busca_por_nome() {
    Roteiro r = comPastoFundo();
    r.cadastrarPasto("Pasto Brejo", "Rio Sul", 120)
     .consultarPastos("")              // 0: todos
     .consultarPastos("Pasto Fundo")   // 1: nome exato
     .consultarPastos("brejo")         // 2: parcial e sem diferenciar maiusculas
     .consultarPastos("Inexistente");  // 3: nada encontrado
    const std::string s = r.executar();

    EXIGE(s, ocorrencias(aposPrompt(s, PROMPT_BUSCA, 0), " | capacidade ") == 2);

    const std::string exato = aposPrompt(s, PROMPT_BUSCA, 1);
    EXIGE(s, ocorrencias(exato, " | capacidade ") == 1);
    EXIGE(s, contem(exato, "Pasto Fundo"));

    const std::string parcial = aposPrompt(s, PROMPT_BUSCA, 2);
    EXIGE(s, ocorrencias(parcial, " | capacidade ") == 1);
    EXIGE(s, contem(parcial, "Pasto Brejo | Rio Sul | capacidade 120"));

    EXIGE(s, contem(aposPrompt(s, PROMPT_BUSCA, 3), "(nenhum pasto encontrado)"));
}

static void teste_consultar_pastos_so_da_propria_fazenda() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(GESTOR, 9, 2)
     .cadastrarPasto("Pasto Alheio", "Longe", 80)
     .consultarPastos()
     .trocarUsuario(FUNCIONARIO, 5, 1)
     .consultarPastos();
    const std::string s = r.executar();

    const std::string fazenda2 = aposPrompt(s, PROMPT_BUSCA, 0);
    EXIGE(s, ocorrencias(fazenda2, " | capacidade ") == 1);
    EXIGE(s, contem(fazenda2, "Pasto Alheio"));

    const std::string fazenda1 = aposPrompt(s, PROMPT_BUSCA, 1);
    EXIGE(s, ocorrencias(fazenda1, " | capacidade ") == 1);
    EXIGE(s, contem(fazenda1, "Pasto Fundo"));
    EXIGE(s, !contem(fazenda1, "Pasto Alheio"));
}

static void teste_consultar_pastos_fazenda_sem_pastos() {
    Roteiro r;
    r.login(FUNCIONARIO, 5, 1).consultarPastos();
    const std::string s = r.executar();
    EXIGE(s, contem(aposPrompt(s, PROMPT_BUSCA), "(nenhum pasto encontrado)"));
}

// =====================================================================
// RF-03  Editar pasto/lote
// =====================================================================

// Cenario 3 e recusas de edicao
static void teste_editar_pasto() {
    Roteiro r = comPastoFundo();
    r.cadastrarPasto("Pasto Brejo", "Rio Sul", 120)
     .editarPasto(1, "Pasto Fundo", "Divisa Norte", 250)  // ok (o proprio nome nao conta como repetido)
     .editarPasto(1, "Pasto Fundo", "Divisa Norte", 0)    // capacidade 0
     .editarPasto(1, "Pasto Brejo", "Divisa Norte", 250)  // nome de outro pasto
     .editarPasto(1, "", "Divisa Norte", 250)             // nome vazio
     .editarPasto(1, "Pasto Fundo", "", 250)              // localizacao vazia
     .editarPasto(999, "Qualquer", "Lugar", 10)           // pasto inexistente
     .consultarPastos("Pasto Fundo");
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true, false, false, false, false, false}));
    const std::string lista = aposPrompt(s, PROMPT_BUSCA);
    EXIGE(s, ocorrencias(lista, " | capacidade ") == 1);
    EXIGE(s, contem(lista, "Pasto Fundo | Divisa Norte | capacidade 250"));  // continua com 250
}

static void teste_editar_pasto_nome_e_localizacao() {
    Roteiro r = comPastoFundo();
    r.editarPasto(1, "Pasto Grande", "Divisa Sul", 300).consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true}));
    const std::string lista = aposPrompt(s, PROMPT_BUSCA);
    EXIGE(s, contem(lista, "[1] Pasto Grande | Divisa Sul | capacidade 300"));
    EXIGE(s, !contem(lista, "Pasto Fundo"));
}

static void teste_editar_pasto_de_outra_fazenda_e_funcionario() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(GESTOR, 9, 2).editarPasto(1, "Pasto Fundo", "Norte", 999)  // outra fazenda
     .trocarUsuario(FUNCIONARIO, 5, 1).editarPasto(1, "Pasto Fundo", "Divisa Norte", 300)  // sem permissao
     .consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, false, false}));
    EXIGE(s, contem(aposPrompt(s, PROMPT_BUSCA), "Pasto Fundo | Divisa Norte | capacidade 200"));
}

// =====================================================================
// RF-04  Remover pasto/lote
// =====================================================================

// Cenario 4: pasto com contagem salva nao pode ser removido
static void teste_remover_pasto() {
    Roteiro r = comPastoFundo();
    r.cadastrarPasto("Pasto Brejo", "Rio Sul", 120)
     .contagemSalva("remocao.png", 185, 1, 185)  // contagem salva no Pasto Fundo
     .removerPasto(1)                            // recusa: tem contagem
     .removerPasto(2)                            // ok: sem contagens
     .removerPasto(2)                            // recusa: ja foi removido
     .removerPasto(999)                          // recusa: nao existe
     .consultarPastos();
    const std::string s = r.executar();

    // cadastros, contar, salvar, e depois as quatro remocoes
    EXIGE(s, resultados(s) == Resp({true, true, true, true, false, true, false, false}));
    const std::string lista = aposPrompt(s, PROMPT_BUSCA);
    EXIGE(s, ocorrencias(lista, " | capacidade ") == 1);
    EXIGE(s, contem(lista, "Pasto Fundo"));
    EXIGE(s, !contem(lista, "Pasto Brejo"));
}

static void teste_remover_pasto_sem_permissao_ou_de_outra_fazenda() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(FUNCIONARIO, 5, 1).removerPasto(1)
     .trocarUsuario(GESTOR, 9, 2).removerPasto(1)
     .trocarUsuario(GESTOR, 1, 1).consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, false, false}));
    EXIGE(s, contem(aposPrompt(s, PROMPT_BUSCA), "Pasto Fundo"));
}

static void teste_remover_pasto_administrador() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(ADMINISTRADOR, 2, 1).removerPasto(1).consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true}));
    EXIGE(s, contem(aposPrompt(s, PROMPT_BUSCA), "(nenhum pasto encontrado)"));
}

// =====================================================================
// RF-05  Contar o gado por foto  (4.2)
// =====================================================================

// Cenario 1 + indicador 2 + nada e salvo ainda
static void teste_contar_foto_principal() {
    Roteiro r;
    r.login(FUNCIONARIO, 5, 1)
     .criarImagem(foto("vistoria_20261006_pasto1.png"), 1920, 1080)
     .contar(foto("vistoria_20261006_pasto1.png"), 185);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true}));
    EXIGE(s, contem(s, "Animais marcados: 185"));
    EXIGE(s, contem(s, "nada foi salvo ainda"));
    EXIGE(s, tempoDeAnalise(s) <= 30.0);
}

// Formatos aceitos: png, jpg e jpeg (sem diferenciar maiusculas)
static void teste_contar_formatos_aceitos() {
    criarJPEG(foto("a.jpg"), 100, 100);
    criarJPEG(foto("b.jpeg"), 100, 100);
    criarJPEG(foto("c.JPG"), 100, 100);

    Roteiro r;
    r.login(FUNCIONARIO, 5, 1)
     .criarImagem(foto("d.png"), 100, 100)
     .criarImagem(foto("e.PNG"), 100, 100)
     .contar(foto("a.jpg"), 10)
     .contar(foto("b.jpeg"), 10)
     .contar(foto("c.JPG"), 10)
     .contar(foto("d.png"), 10)
     .contar(foto("e.PNG"), 10);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true, true, true}));
}

// Cenario 2: formato nao aceito
static void teste_contar_formato_recusado() {
    criarTexto(foto("relatorio.pdf"), "%PDF-1.4");
    criarTexto(foto("foto.bmp"), "BM");
    criarTexto(foto("foto.gif"), "GIF89a");
    criarTexto(foto("semextensao"), "x");

    Roteiro r;
    r.login(FUNCIONARIO, 5, 1)
     .contar(foto("relatorio.pdf"), 185)
     .contar(foto("foto.bmp"), 185)
     .contar(foto("foto.gif"), 185)
     .contar(foto("semextensao"), 185);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({false, false, false, false}));
    EXIGE(s, !contem(s, "Animais marcados"));
}

static void teste_contar_arquivo_inexistente_ou_invalido() {
    criarTexto(foto("falso.png"), "isto nao e uma imagem");
    criarTexto(foto("vazio.png"), "");
    fs::create_directories(pasta() / "pasta.png");  // uma pasta nao e uma imagem

    Roteiro r;
    r.login(FUNCIONARIO, 5, 1)
     .contar(foto("nao_existe.png"), 185)
     .contar("", 185)
     .contar(foto("falso.png"), 185)
     .contar(foto("vazio.png"), 185)
     .contar(foto("pasta.png"), 185);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({false, false, false, false, false}));
    EXIGE(s, !contem(s, "Animais marcados"));
}

// Limite de 4K: ate 3840x2160 (ou 2160x3840)
static void teste_contar_limite_4k() {
    Roteiro r;
    r.login(FUNCIONARIO, 5, 1)
     .criarImagem(foto("uhd.png"), 3840, 2160)
     .criarImagem(foto("retrato.png"), 2160, 3840)
     .criarImagem(foto("largura.png"), 3841, 2160)
     .criarImagem(foto("altura.png"), 3840, 2161)
     .criarImagem(foto("8k.png"), 7680, 4320)
     .contar(foto("uhd.png"), 10)
     .contar(foto("retrato.png"), 10)
     .contar(foto("largura.png"), 10)
     .contar(foto("altura.png"), 10)
     .contar(foto("8k.png"), 10);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, false, false, false}));
    EXIGE(s, ocorrencias(s, "Animais marcados: 10") == 2);
}

// Cenario 3: pasto sem animais mostra total 0 (resultado valido)
static void teste_contar_foto_sem_animais() {
    Roteiro r;
    r.login(FUNCIONARIO, 5, 1)
     .criarImagem(foto("vazio_pasto.png"), 640, 480)
     .contar(foto("vazio_pasto.png"), 0);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true}));
    EXIGE(s, contem(s, "Animais marcados: 0"));
}

// Funcionario, Gestor e Administrador podem contar
static void teste_contar_todos_os_perfis() {
    Roteiro r;
    r.login(FUNCIONARIO, 5, 1).criarImagem(foto("perfis.png"), 800, 600).contar(foto("perfis.png"), 40)
     .trocarUsuario(GESTOR, 1, 1).contar(foto("perfis.png"), 41)
     .trocarUsuario(ADMINISTRADOR, 2, 1).contar(foto("perfis.png"), 42);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true}));
    EXIGE(s, contem(s, "Animais marcados: 40"));
    EXIGE(s, contem(s, "Animais marcados: 41"));
    EXIGE(s, contem(s, "Animais marcados: 42"));
}

// Uma contagem recusada nao altera a ultima contagem valida
static void teste_contar_recusa_preserva_contagem_anterior() {
    criarTexto(foto("relatorio2.pdf"), "%PDF-1.4");

    Roteiro r = comPastoFundo();
    r.criarImagem(foto("boa.png"), 800, 600)
     .contar(foto("boa.png"), 185)
     .contar(foto("relatorio2.pdf"), 50)  // recusada
     .salvar(1, 185);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, false, true}));
    EXIGE(s, contem(s, "Registro 1 salvo: calculado 185, confirmado 185"));
}

// =====================================================================
// RF-06  Salvar a contagem  (4.3)
// =====================================================================

// Cenario 1: Funcionario Joao salva 185 contados e 185 confirmados
static void teste_salvar_contagem_principal() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(FUNCIONARIO, 5, 1)
     .contagemSalva("vistoria_20261006_pasto1.png", 185, 1, 185)
     .trocarUsuario(GESTOR, 1, 1)
     .historico(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true}));
    EXIGE(s, contem(s, "Cancelado") == false);
    EXIGE(s, contem(s, "Registro 1 salvo: calculado 185, confirmado 185"));

    // quem contou, pasto, totais e foto marcada aparecem no historico
    const std::string hist = aposPrompt(s, PROMPT_FUNC);
    EXIGE(s, contem(hist, "| operador 5 | pasto 1 | calculado 185 | confirmado 185 | "
                          + foto("vistoria_20261006_pasto1.png")));
}

// Cenario 2: o usuario corrige o total
static void teste_salvar_contagem_com_correcao() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("correcao.png", 180, 1, 200);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true}));
    EXIGE(s, contem(s, "Registro 1 salvo: calculado 180, confirmado 200"));
}

// Cenario 3 e demais recusas: nada e salvo e o pasto continua removivel
static void teste_salvar_contagem_recusas() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(GESTOR, 9, 2).cadastrarPasto("Pasto Alheio", "Longe", 80)   // pasto 2, da fazenda 2
     .trocarUsuario(FUNCIONARIO, 5, 1)
     .criarImagem(foto("recusas.png"), 800, 600)
     .contar(foto("recusas.png"), 185)
     .salvar(999, 185)   // pasto inexistente
     .salvar(2, 185)     // pasto de outra fazenda
     .salvar(1, -1)      // total negativo
     .trocarUsuario(GESTOR, 1, 1)
     .historico(TODO_PERIODO)
     .removerPasto(1);   // recusas nao bloqueiam a remocao do pasto
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true, false, false, false, true}));
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC), "(nenhuma contagem encontrada ou sem permissao)"));
}

static void teste_salvar_contagem_cancelada() {
    Roteiro r = comPastoFundo();
    r.criarImagem(foto("cancelar.png"), 800, 600)
     .contar(foto("cancelar.png"), 185)
     .salvar(1, 185, false)
     .historico(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true}));  // so o cadastro e a contagem
    EXIGE(s, contem(s, "Cancelado. Nada foi salvo."));
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC), "(nenhuma contagem encontrada ou sem permissao)"));
}

static void teste_salvar_sem_ter_contado() {
    Roteiro r = comPastoFundo();
    r.salvarSemContagem();
    const std::string s = r.executar();

    EXIGE(s, contem(s, "Nenhuma contagem para salvar. Conte uma foto primeiro."));
    EXIGE(s, resultados(s) == Resp({true}));
}

// Foto sem animais: total 0 e valido e pode ser salvo
static void teste_salvar_contagem_zero() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("zero.png", 0, 1, 0);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true}));
    EXIGE(s, contem(s, "Registro 1 salvo: calculado 0, confirmado 0"));
}

// Gestor e Administrador tambem salvam; o operador registrado e quem esta logado
static void teste_salvar_gestor_e_administrador() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("g.png", 10, 1, 10)
     .trocarUsuario(ADMINISTRADOR, 2, 1)
     .contagemSalva("a.png", 20, 1, 20)
     .historico(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true, true, true}));
    const std::string hist = aposPrompt(s, PROMPT_FUNC);
    EXIGE(s, contem(hist, "| operador 1 | pasto 1 | calculado 10 | confirmado 10 |"));
    EXIGE(s, contem(hist, "| operador 2 | pasto 1 | calculado 20 | confirmado 20 |"));
}

// =====================================================================
// RF-07  Consultar o historico e o erro da contagem  (4.4)
// =====================================================================

// Cenario 1: 185/185 e 180/200 resultam em erro de 5%
static void teste_historico_e_erro_principal() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(FUNCIONARIO, 5, 1)
     .contagemSalva("a.png", 185, 1, 185)
     .contagemSalva("b.png", 180, 1, 200)
     .trocarUsuario(GESTOR, 1, 1)
     .historico(TODO_PERIODO)
     .erro(TODO_PERIODO);
    const std::string s = r.executar();

    const std::string hist = aposPrompt(s, PROMPT_FUNC, 0);
    EXIGE(s, ocorrencias(hist, "| operador 5 |") == 2);
    EXIGE(s, contem(hist, "calculado 185 | confirmado 185"));
    EXIGE(s, contem(hist, "calculado 180 | confirmado 200"));

    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Erro medio da contagem: 5%"));
}

// Cenario 2: o Funcionario nao ve nenhuma informacao
static void teste_historico_funcionario_nao_consulta() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(FUNCIONARIO, 5, 1)
     .contagemSalva("a.png", 185, 1, 185)
     .historico(TODO_PERIODO)
     .erro(TODO_PERIODO);
    const std::string s = r.executar();

    const std::string hist = aposPrompt(s, PROMPT_FUNC, 0);
    EXIGE(s, contem(hist, "(nenhuma contagem encontrada ou sem permissao)"));
    EXIGE(s, ocorrencias(hist, "| operador") == 0);
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Nao foi possivel calcular"));
}

// Cenario 3: sem contagens no periodo, lista vazia e erro nao calculado
static void teste_historico_periodo_sem_contagens() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("a.png", 185, 1, 185)
     .historico(ANO_2020)
     .erro(ANO_2020);
    const std::string s = r.executar();

    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 0), "(nenhuma contagem encontrada ou sem permissao)"));
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Nao foi possivel calcular"));
}

// Fazenda sem nenhuma contagem
static void teste_historico_sem_nenhuma_contagem() {
    Roteiro r;
    r.login(GESTOR, 1, 1).historico(TODO_PERIODO).erro(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 0), "(nenhuma contagem encontrada ou sem permissao)"));
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Nao foi possivel calcular"));
}

// O Administrador consulta como o Gestor
static void teste_historico_administrador() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("a.png", 185, 1, 185)
     .contagemSalva("b.png", 180, 1, 200)
     .trocarUsuario(ADMINISTRADOR, 2, 1)
     .historico(TODO_PERIODO)
     .erro(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, ocorrencias(aposPrompt(s, PROMPT_FUNC, 0), "| operador 1 |") == 2);
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Erro medio da contagem: 5%"));
}

// Cada fazenda ve so o que e seu
static void teste_historico_isolado_por_fazenda() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(FUNCIONARIO, 5, 1)
     .contagemSalva("a.png", 185, 1, 185)
     .trocarUsuario(GESTOR, 9, 2)
     .historico(TODO_PERIODO)
     .erro(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 0), "(nenhuma contagem encontrada ou sem permissao)"));
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Nao foi possivel calcular"));
}

// Filtros por pasto e por funcionario
static void teste_historico_filtros_por_pasto_e_funcionario() {
    Roteiro r = comPastoFundo();
    r.cadastrarPasto("Pasto Brejo", "Rio Sul", 120)           // pasto 2
     .trocarUsuario(FUNCIONARIO, 5, 1)
     .contagemSalva("f.png", 185, 1, 185)                     // operador 5, pasto 1
     .trocarUsuario(GESTOR, 1, 1)
     .contagemSalva("g.png", 90, 2, 90)                       // operador 1, pasto 2
     .historico(TODO_PERIODO)        // 0: todos
     .historico(TODO_PERIODO, 2, 0)  // 1: so o pasto 2
     .historico(TODO_PERIODO, 0, 5)  // 2: so o funcionario 5
     .historico(TODO_PERIODO, 1, 5)  // 3: pasto 1 e funcionario 5
     .historico(TODO_PERIODO, 2, 5); // 4: nenhum
    const std::string s = r.executar();

    EXIGE(s, ocorrencias(aposPrompt(s, PROMPT_FUNC, 0), "| operador") == 2);

    const std::string doPasto2 = aposPrompt(s, PROMPT_FUNC, 1);
    EXIGE(s, ocorrencias(doPasto2, "| operador") == 1);
    EXIGE(s, contem(doPasto2, "| operador 1 | pasto 2 |"));

    const std::string doFunc5 = aposPrompt(s, PROMPT_FUNC, 2);
    EXIGE(s, ocorrencias(doFunc5, "| operador") == 1);
    EXIGE(s, contem(doFunc5, "| operador 5 | pasto 1 |"));

    EXIGE(s, ocorrencias(aposPrompt(s, PROMPT_FUNC, 3), "| operador") == 1);
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 4), "(nenhuma contagem encontrada ou sem permissao)"));
}

// O erro conta a diferenca nos dois sentidos (220/200 e 180/200 dao 10% cada)
static void teste_erro_diferenca_absoluta() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("a.png", 220, 1, 200)
     .contagemSalva("b.png", 180, 1, 200)
     .erro(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC), ">> Erro medio da contagem: 10%"));
}

// Contagens com total confirmado 0 ficam fora do calculo, mas aparecem na lista
static void teste_erro_ignora_confirmado_zero() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("a.png", 3, 1, 0)
     .contagemSalva("b.png", 180, 1, 200)
     .historico(TODO_PERIODO)
     .erro(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, ocorrencias(aposPrompt(s, PROMPT_FUNC, 0), "| operador") == 2);
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Erro medio da contagem: 10%"));
}

static void teste_erro_so_com_confirmado_zero() {
    Roteiro r = comPastoFundo();
    r.contagemSalva("a.png", 0, 1, 0).erro(TODO_PERIODO);
    const std::string s = r.executar();

    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC), ">> Nao foi possivel calcular"));
}

// =====================================================================
// Fluxo completo (3.3) e indicadores (1.1)
// =====================================================================

// O Gestor cadastra, o Funcionario conta e salva, o Gestor consulta e mede o erro.
static void teste_fluxo_completo() {
    Roteiro r;
    r.login(GESTOR, 1, 1)
     .cadastrarPasto("Pasto Fundo", "Divisa Norte", 200)     // 1) cadastra
     .trocarUsuario(FUNCIONARIO, 5, 1)
     .consultarPastos("Pasto Fundo")                         // 2) escolhe o pasto
     .criarImagem(foto("fluxo.png"), 3840, 2160)
     .contar(foto("fluxo.png"), 185)                         // 3) conta por foto
     .salvar(1, 185)                                         // 4) confirma e salva
     .trocarUsuario(GESTOR, 1, 1)
     .historico(TODO_PERIODO)                                // 5) consulta
     .erro(TODO_PERIODO)                                     // 6) mede o erro
     .removerPasto(1);                                       // 7) pasto com contagem nao sai
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, true, true, false}));
    EXIGE(s, contem(aposPrompt(s, PROMPT_BUSCA), "[1] Pasto Fundo"));
    EXIGE(s, contem(s, "Animais marcados: 185"));
    EXIGE(s, contem(s, "Registro 1 salvo: calculado 185, confirmado 185"));
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 0), "| operador 5 | pasto 1 | calculado 185 | confirmado 185 |"));
    EXIGE(s, contem(aposPrompt(s, PROMPT_FUNC, 1), ">> Erro medio da contagem: 0%"));
}

// Indicador 1: nenhum pasto repetido e todos com capacidade informada
static void teste_indicador_pastos_cadastrados_corretamente() {
    Roteiro r;
    r.login(GESTOR, 1, 1)
     .cadastrarPasto("Pasto A", "Norte", 100)
     .cadastrarPasto("Pasto A", "Sul", 100)   // repetido
     .cadastrarPasto("Pasto B", "Sul", 0)     // sem capacidade
     .cadastrarPasto("Pasto C", "", 50)       // incompleto
     .consultarPastos();
    const std::string s = r.executar();

    EXIGE(s, resultados(s) == Resp({true, false, false, false}));
    EXIGE(s, ocorrencias(aposPrompt(s, PROMPT_BUSCA), " | capacidade ") == 1);
}

// Indicador 3: toda contagem salva traz quem contou, data e hora, pasto, totais e foto
static void teste_indicador_contagem_registrada_por_completo() {
    Roteiro r = comPastoFundo();
    r.trocarUsuario(FUNCIONARIO, 5, 1)
     .contagemSalva("completo.png", 180, 1, 200)
     .trocarUsuario(GESTOR, 1, 1)
     .historico(TODO_PERIODO);
    const std::string s = r.executar();

    const std::string hist = aposPrompt(s, PROMPT_FUNC);
    EXIGE(s, contem(hist, "[1] "));                                   // identificador
    EXIGE(s, contem(hist, "/") && contem(hist, ":"));                 // data (dd/mm/aaaa) e hora (hh:mm:ss)
    EXIGE(s, contem(hist, "| operador 5 |"));                         // quem contou
    EXIGE(s, contem(hist, "| pasto 1 |"));                            // pasto
    EXIGE(s, contem(hist, "| calculado 180 |"));                      // total calculado
    EXIGE(s, contem(hist, "| confirmado 200 |"));                     // total confirmado
    EXIGE(s, contem(hist, foto("completo.png")));                     // foto marcada
}

// =====================================================================

int main() {
    std::error_code erro;
    fs::remove_all(pasta(), erro);
    fs::create_directories(pasta());

    std::cout << "===== TESTES FUNCIONAIS =====" << std::endl;

    std::cout << "[RF-00] Acesso e navegacao" << std::endl;
    RODAR(teste_acesso_login_e_troca_de_usuario);
    RODAR(teste_acesso_opcao_invalida_do_menu);

    std::cout << "\n[RF-01] Cadastrar pasto/lote" << std::endl;
    RODAR(teste_cadastrar_pasto_principal);
    RODAR(teste_cadastrar_pasto_recusas);
    RODAR(teste_cadastrar_pasto_nome_em_outra_fazenda);
    RODAR(teste_cadastrar_pasto_funcionario_recusado);
    RODAR(teste_cadastrar_pasto_administrador);

    std::cout << "\n[RF-02] Consultar pastos/lotes" << std::endl;
    RODAR(teste_consultar_pastos_busca_por_nome);
    RODAR(teste_consultar_pastos_so_da_propria_fazenda);
    RODAR(teste_consultar_pastos_fazenda_sem_pastos);

    std::cout << "\n[RF-03] Editar pasto/lote" << std::endl;
    RODAR(teste_editar_pasto);
    RODAR(teste_editar_pasto_nome_e_localizacao);
    RODAR(teste_editar_pasto_de_outra_fazenda_e_funcionario);

    std::cout << "\n[RF-04] Remover pasto/lote" << std::endl;
    RODAR(teste_remover_pasto);
    RODAR(teste_remover_pasto_sem_permissao_ou_de_outra_fazenda);
    RODAR(teste_remover_pasto_administrador);

    std::cout << "\n[RF-05] Contar o gado por foto" << std::endl;
    RODAR(teste_contar_foto_principal);
    RODAR(teste_contar_formatos_aceitos);
    RODAR(teste_contar_formato_recusado);
    RODAR(teste_contar_arquivo_inexistente_ou_invalido);
    RODAR(teste_contar_limite_4k);
    RODAR(teste_contar_foto_sem_animais);
    RODAR(teste_contar_todos_os_perfis);
    RODAR(teste_contar_recusa_preserva_contagem_anterior);

    std::cout << "\n[RF-06] Salvar a contagem" << std::endl;
    RODAR(teste_salvar_contagem_principal);
    RODAR(teste_salvar_contagem_com_correcao);
    RODAR(teste_salvar_contagem_recusas);
    RODAR(teste_salvar_contagem_cancelada);
    RODAR(teste_salvar_sem_ter_contado);
    RODAR(teste_salvar_contagem_zero);
    RODAR(teste_salvar_gestor_e_administrador);

    std::cout << "\n[RF-07] Historico e erro da contagem" << std::endl;
    RODAR(teste_historico_e_erro_principal);
    RODAR(teste_historico_funcionario_nao_consulta);
    RODAR(teste_historico_periodo_sem_contagens);
    RODAR(teste_historico_sem_nenhuma_contagem);
    RODAR(teste_historico_administrador);
    RODAR(teste_historico_isolado_por_fazenda);
    RODAR(teste_historico_filtros_por_pasto_e_funcionario);
    RODAR(teste_erro_diferenca_absoluta);
    RODAR(teste_erro_ignora_confirmado_zero);
    RODAR(teste_erro_so_com_confirmado_zero);

    std::cout << "\n[Fluxo e indicadores]" << std::endl;
    RODAR(teste_fluxo_completo);
    RODAR(teste_indicador_pastos_cadastrados_corretamente);
    RODAR(teste_indicador_contagem_registrada_por_completo);

    fs::remove_all(pasta(), erro);
    std::cout << "  test_funcional: OK" << std::endl;
    std::cout << "=============================" << std::endl;
    return 0;
}