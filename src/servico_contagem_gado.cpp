#include "servico_contagem_gado.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

namespace contagem_gado {

namespace {

// Interpretacao adotada para "ate 4K": UHD 3840x2160 (ou 2160x3840).

constexpr std::uint32_t LIMITE_4K_MAIOR = 3840;
constexpr std::uint32_t LIMITE_4K_MENOR = 2160;

std::uint32_t lerBE32(const unsigned char* b) {
    return (static_cast<std::uint32_t>(b[0]) << 24) |
           (static_cast<std::uint32_t>(b[1]) << 16) |
           (static_cast<std::uint32_t>(b[2]) << 8) |
           static_cast<std::uint32_t>(b[3]);
}

std::uint16_t lerBE16(const unsigned char* b) {
    return static_cast<std::uint16_t>((b[0] << 8) | b[1]);
}

// Le as dimensoes do cabecalho PNG sem bibliotecas adicionais.
bool dimensoesPNG(std::ifstream& arquivo, std::uint32_t& largura,
                  std::uint32_t& altura) {
    std::array<unsigned char, 24> cabecalho{};
    if (!arquivo.read(reinterpret_cast<char*>(cabecalho.data()),
                      static_cast<std::streamsize>(cabecalho.size()))) {
        return false;
    }

    constexpr std::array<unsigned char, 8> assinatura =
        {137, 80, 78, 71, 13, 10, 26, 10};
    if (!std::equal(assinatura.begin(), assinatura.end(), cabecalho.begin())) {
        return false;
    }

    if (lerBE32(cabecalho.data() + 8) != 13 ||
        cabecalho[12] != 'I' || cabecalho[13] != 'H' ||
        cabecalho[14] != 'D' || cabecalho[15] != 'R') {
        return false;
    }

    largura = lerBE32(cabecalho.data() + 16);
    altura = lerBE32(cabecalho.data() + 20);
    return largura > 0 && altura > 0;
}

bool marcadorComDimensoes(unsigned char marcador) {
    return (marcador >= 0xC0 && marcador <= 0xC3) ||
           (marcador >= 0xC5 && marcador <= 0xC7) ||
           (marcador >= 0xC9 && marcador <= 0xCB) ||
           (marcador >= 0xCD && marcador <= 0xCF);
}

bool dimensoesJPEG(std::ifstream& arquivo, std::uint32_t& largura,
                   std::uint32_t& altura) {
    unsigned char inicio[2]{};
    if (!arquivo.read(reinterpret_cast<char*>(inicio), 2) ||
        inicio[0] != 0xFF || inicio[1] != 0xD8) {
        return false;
    }

    while (true) {
        int inicioSegmento = arquivo.get();
        if (inicioSegmento != 0xFF) return false;

        int marcador = arquivo.get();
        while (marcador == 0xFF) marcador = arquivo.get();
        if (marcador == EOF || marcador == 0x00 ||
            marcador == 0xD9 || marcador == 0xDA) {
            return false;
        }

        // Marcadores sem corpo (RSTn e TEM) nao possuem comprimento.
        if ((marcador >= 0xD0 && marcador <= 0xD7) || marcador == 0x01) {
            continue;
        }

        unsigned char bytesComprimento[2]{};
        if (!arquivo.read(reinterpret_cast<char*>(bytesComprimento), 2)) {
            return false;
        }
        std::uint16_t comprimento = lerBE16(bytesComprimento);
        if (comprimento < 2) return false;

        if (marcadorComDimensoes(static_cast<unsigned char>(marcador))) {
            // SOF: comprimento(2), precisao(1), altura(2), largura(2)...
            if (comprimento < 7) return false;
            unsigned char dados[5]{};
            if (!arquivo.read(reinterpret_cast<char*>(dados), 5)) {
                return false;
            }
            altura = lerBE16(dados + 1);
            largura = lerBE16(dados + 3);
            return largura > 0 && altura > 0;
        }

        arquivo.seekg(static_cast<std::streamoff>(comprimento) - 2,
                      std::ios::cur);
        if (!arquivo) return false;
    }
}

bool imagemDentroDe4K(const std::filesystem::path& arquivo,
                      const std::string& extensao) {
    std::ifstream entrada(arquivo, std::ios::binary);
    if (!entrada) return false;

    std::uint32_t largura = 0;
    std::uint32_t altura = 0;
    const bool conseguiuLer = (extensao == ".png")
        ? dimensoesPNG(entrada, largura, altura)
        : dimensoesJPEG(entrada, largura, altura);

    if (!conseguiuLer) return false;
    return std::max(largura, altura) <= LIMITE_4K_MAIOR &&
           std::min(largura, altura) <= LIMITE_4K_MENOR;
}

} // namespace

ServicoContagem::ServicoContagem(const Sessao& sessao, MotorVisao& motor)
    : sessao_(sessao), motor_(motor) {}

bool ServicoContagem::countByImage(const Imagem& imagem, Contagem& contagem) {
    const std::string papel = sessao_.user().role();
    if (papel != "Gestor" && papel != "Funcionário" &&
        papel != "Administrador") {
        return false;
    }

    const std::filesystem::path arquivo(imagem.path());
    std::error_code erro;
    if (!std::filesystem::is_regular_file(arquivo, erro)) {
        return false;
    }

    std::string extensao = arquivo.extension().string();
    std::transform(extensao.begin(), extensao.end(), extensao.begin(),
                   [](unsigned char c) {
                       return static_cast<char>(std::tolower(c));
                   });
    if (extensao != ".jpg" && extensao != ".jpeg" && extensao != ".png") {
        return false;
    }

    // Nao envia imagens acima de 4K (ou com cabecalho invalido) para o motor.
    if (!imagemDentroDe4K(arquivo, extensao)) {
        return false;
    }

    // Operacao transacional: se o motor falhar, a contagem de entrada
    // permanece exatamente como estava, mesmo que o motor escreva dados.
    Contagem temporaria;
    if (!motor_.detect(imagem, temporaria)) {
        return false;
    }

    contagem = temporaria;
    return true;
}

} // namespace contagem_gado
