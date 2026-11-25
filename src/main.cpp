#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "usuario/Usuario.hpp"
#include "usuario/Sessao.hpp"
#include "pasto/ServicoPastos.hpp"
#include "historico/ServicoHistorico.hpp"
#include "contagem_gado/servico_contagem_gado.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// ---------- Motor de IA simulado ----------
// Ainda nao ha motor real (YOLO/OpenCV), entao este motor "detecta"
// sempre a quantidade de animais que o usuario escolheu.
class MotorSimulado : public contagem_gado::MotorVisao {
public:
    int animais;

    MotorSimulado() : animais(0) {}

    bool detect(const Imagem& imagem, Contagem& contagem) {
        Colecao<Retangulo> caixas;
        for (int i = 0; i < animais; ++i) {
            caixas.add(Retangulo(i * 10, i * 10, 10, 10));
        }
        contagem = Contagem(imagem, caixas, 0.5);
        return true;
    }
};

// ---------- Estado do programa ----------
Usuario usuarioAtual;                      // quem esta logado
MotorSimulado motor;                       // motor de IA simulado
Contagem ultimaContagem;                   // resultado da ultima foto analisada
bool temUltimaContagem = false;            // ja houve alguma contagem?

// ---------- Leitura de dados ----------
string lerTexto(const string& pergunta) {
    string linha;
    cout << pergunta;
    if (!std::getline(cin, linha)) {
        cout << "\nEntrada encerrada." << endl;
        std::exit(0);
    }
    return linha;
}

int lerInt(const string& pergunta) {
    string linha = lerTexto(pergunta);
    return std::atoi(linha.c_str());   // texto invalido vira 0
}

bool lerSimNao(const string& pergunta) {
    string r = lerTexto(pergunta + " (s/n): ");
    return r == "s" || r == "S";
}

// ---------- Apoio ----------
void mostrarResultado(bool ok) {
    if (ok) {
        cout << ">> OK: operacao realizada." << endl;
    } else {
        cout << ">> RECUSADO: sem permissao, dados invalidos ou item inexistente." << endl;
    }
}

void mostrarPasto(const Pasto& p) {
    cout << "  [" << p.id() << "] " << p.name()
         << " | " << p.location()
         << " | capacidade " << p.capacity() << endl;
}

void listarPastos(const string& busca) {
    Sessao sessao(usuarioAtual);
    ServicoPastos pastos(sessao);
    Colecao<Pasto> lista = pastos.query(FiltroPasto(busca));

    if (lista.size() == 0) {
        cout << "  (nenhum pasto encontrado)" << endl;
        return;
    }
    for (Iterador<Pasto> it = lista.iterator(); it.hasNext(); ) {
        mostrarPasto(it.next());
    }
}

Data lerData(const string& titulo) {
    cout << titulo << endl;
    int dia = lerInt("  dia: ");
    int mes = lerInt("  mes: ");
    int ano = lerInt("  ano: ");
    return Data(ano, mes, dia);
}

FiltroHistorico lerFiltroHistorico() {
    Data inicio = lerData("Data inicial:");
    Data fim = lerData("Data final:");
    int idPasto = lerInt("Id do pasto (0 = todos): ");
    int idOperador = lerInt("Id do funcionario (0 = todos): ");
    return FiltroHistorico(inicio, fim, idPasto, idOperador);
}

// ---------- Login ----------
void fazerLogin() {
    cout << "\n=== LOGIN ===" << endl;
    cout << "1 - Gestor" << endl;
    cout << "2 - Funcionario" << endl;
    cout << "3 - Administrador" << endl;

    int opcao = 0;
    while (opcao < 1 || opcao > 3) {
        opcao = lerInt("Escolha o papel: ");
    }

    string papel;
    if (opcao == 1) papel = "Gestor";
    if (opcao == 2) papel = "Funcionário";
    if (opcao == 3) papel = "Administrador";

    int id = lerInt("Id do usuario: ");
    int fazenda = lerInt("Id da fazenda: ");

    usuarioAtual = Usuario(id, papel, fazenda);
    cout << ">> Logado como " << papel << " (id " << id
         << ", fazenda " << fazenda << ")" << endl;
}

// ---------- Pastos ----------
void cadastrarPasto() {
    string nome = lerTexto("Nome: ");
    string local = lerTexto("Localizacao: ");
    int capacidade = lerInt("Capacidade maxima: ");

    Sessao sessao(usuarioAtual);
    ServicoPastos pastos(sessao);
    Pasto novo;
    bool ok = pastos.create(DadosPasto(nome, local, capacidade), novo);
    mostrarResultado(ok);
    if (ok) {
        mostrarPasto(novo);
    }
}

void editarPasto() {
    listarPastos("");
    int id = lerInt("Id do pasto a editar: ");
    string nome = lerTexto("Novo nome: ");
    string local = lerTexto("Nova localizacao: ");
    int capacidade = lerInt("Nova capacidade: ");

    Sessao sessao(usuarioAtual);
    ServicoPastos pastos(sessao);
    mostrarResultado(pastos.update(id, DadosPasto(nome, local, capacidade)));
}

void removerPasto() {
    listarPastos("");
    int id = lerInt("Id do pasto a remover: ");

    Sessao sessao(usuarioAtual);
    ServicoPastos pastos(sessao);
    mostrarResultado(pastos.remove(id));
}

// ---------- Imagem de teste ----------
// Escreve um arquivo com cabecalho PNG valido (so assinatura + IHDR) nas
// dimensoes pedidas. Serve para testar o limite de 4K sem uma foto real.
void escreverInt32(std::ofstream& arq, unsigned int n) {
    arq.put(static_cast<char>((n >> 24) & 0xFF));
    arq.put(static_cast<char>((n >> 16) & 0xFF));
    arq.put(static_cast<char>((n >> 8) & 0xFF));
    arq.put(static_cast<char>(n & 0xFF));
}

void criarImagemTeste() {
    string nome = lerTexto("Nome do arquivo (ex: teste.png): ");
    int largura = lerInt("Largura em pixels: ");
    int altura = lerInt("Altura em pixels: ");

    std::ofstream arq(nome.c_str(), std::ios::binary);
    if (!arq) {
        cout << ">> Nao foi possivel criar o arquivo." << endl;
        return;
    }

    const unsigned char assinatura[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    for (int i = 0; i < 8; ++i) arq.put(static_cast<char>(assinatura[i]));
    escreverInt32(arq, 13);
    arq.put('I'); arq.put('H'); arq.put('D'); arq.put('R');
    escreverInt32(arq, static_cast<unsigned int>(largura));
    escreverInt32(arq, static_cast<unsigned int>(altura));
    const unsigned char resto[9] = {8, 2, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 9; ++i) arq.put(static_cast<char>(resto[i]));

    cout << ">> Arquivo '" << nome << "' criado (" << largura << "x" << altura << ")." << endl;
}

// ---------- Contagem ----------
void contarGado() {
    string caminho = lerTexto("Caminho da imagem (jpeg, jpg ou png): ");
    motor.animais = lerInt("Quantos animais o motor simulado deve detectar? ");

    Sessao sessao(usuarioAtual);
    contagem_gado::ServicoContagem servico(sessao, motor);

    Contagem resultado;
    bool ok = servico.countByImage(Imagem(caminho), resultado);
    mostrarResultado(ok);

    if (ok) {
        ultimaContagem = resultado;
        temUltimaContagem = true;
        cout << "  Animais marcados: " << resultado.total() << endl;
        cout << "  Tempo de analise: " << resultado.processingSeconds() << " s" << endl;
        cout << "  (nada foi salvo ainda; use a opcao de salvar)" << endl;
    }
}

void salvarContagem() {
    if (!temUltimaContagem) {
        cout << ">> Nenhuma contagem para salvar. Conte uma foto primeiro." << endl;
        return;
    }

    cout << "Contagem calculada: " << ultimaContagem.total() << " animais" << endl;
    listarPastos("");
    int idPasto = lerInt("Id do pasto contado: ");
    int confirmado = lerInt("Total confirmado (pode corrigir o calculado): ");

    if (!lerSimNao("Confirma o salvamento?")) {
        cout << ">> Cancelado. Nada foi salvo." << endl;
        return;
    }

    Sessao sessao(usuarioAtual);
    ServicoHistorico historico(sessao);
    Registro r;
    bool ok = historico.save(ultimaContagem, idPasto, confirmado, r);
    mostrarResultado(ok);
    
    if (ok) {
        cout << "  Registro " << r.id() << " salvo: calculado "
             << r.automaticTotal() << ", confirmado " << r.confirmedTotal() << endl;
    }
}

// Escreve um numero com 2 digitos (ex: 4 vira 04).
string doisDigitos(int n) {
    if (n < 10) {
        return "0" + std::to_string(n);
    }
    return std::to_string(n);
}

// ---------- Historico ----------
void consultarHistorico() {
    FiltroHistorico filtro = lerFiltroHistorico();

    Sessao sessao(usuarioAtual);
    ServicoHistorico historico(sessao);
    Colecao<Registro> lista = historico.query(filtro);

    if (lista.size() == 0) {
        cout << "  (nenhuma contagem encontrada ou sem permissao)" << endl;
        return;
    }

    for (Iterador<Registro> it = lista.iterator(); it.hasNext(); ) {
        const Registro& r = it.next();
        DataHora dh = r.dateTime();
        cout << "  [" << r.id() << "] "
             << doisDigitos(dh.date().day()) << "/"
             << doisDigitos(dh.date().month()) << "/"
             << dh.date().year() << " "
             << doisDigitos(dh.hour()) << ":"
             << doisDigitos(dh.minute()) << ":"
             << doisDigitos(dh.second())
             << " | operador " << r.operatorId()
             << " | pasto " << r.pastureId()
             << " | calculado " << r.automaticTotal()
             << " | confirmado " << r.confirmedTotal()
             << " | " << r.imagePath() << endl;
    }
}

void calcularErro() {
    FiltroHistorico filtro = lerFiltroHistorico();

    Sessao sessao(usuarioAtual);
    ServicoHistorico historico(sessao);
    double taxa = 0.0;
    if (historico.errorRate(filtro, taxa)) {
        cout << ">> Erro medio da contagem: " << taxa << "%" << endl;
    } else {
        cout << ">> Nao foi possivel calcular (sem permissao ou sem contagens no periodo)." << endl;
    }
}

// ---------- Menu ----------
void mostrarMenu() {
    cout << "\n===== BOVICOUNT =====" << endl;
    cout << "Usuario: " << usuarioAtual.role() << " (id " << usuarioAtual.id()
         << ", fazenda " << usuarioAtual.propertyId() << ")" << endl;
    cout << " 1 - Trocar de usuario" << endl;
    cout << " 2 - Cadastrar pasto" << endl;
    cout << " 3 - Consultar pastos" << endl;
    cout << " 4 - Editar pasto" << endl;
    cout << " 5 - Remover pasto" << endl;
    cout << " 6 - Criar imagem de teste" << endl;
    cout << " 7 - Contar gado por foto" << endl;
    cout << " 8 - Salvar ultima contagem" << endl;
    cout << " 9 - Consultar historico" << endl;
    cout << "10 - Erro da contagem" << endl;
    cout << " 0 - Sair" << endl;
}

int main() {
    cout << "Bem-vindo ao BoviCount (execucao interativa)" << endl;
    fazerLogin();

    int opcao = -1;
    while (opcao != 0) {
        mostrarMenu();
        opcao = lerInt("Opcao: ");
        cout << endl;

        switch (opcao) {
            case 1:  fazerLogin(); break;
            case 2:  cadastrarPasto(); break;
            case 3:  listarPastos(lerTexto("Buscar pelo nome (vazio = todos): ")); break;
            case 4:  editarPasto(); break;
            case 5:  removerPasto(); break;
            case 6:  criarImagemTeste(); break;
            case 7:  contarGado(); break;
            case 8:  salvarContagem(); break;
            case 9:  consultarHistorico(); break;
            case 10: calcularErro(); break;
            case 0:  cout << "Ate logo!" << endl; break;
            default: cout << "Opcao invalida." << endl;
        }
    }
    return 0;
}