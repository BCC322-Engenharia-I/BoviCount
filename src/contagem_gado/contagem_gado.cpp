#include "contagem_gado.h"

namespace contagem_gado {

Imagem::Imagem() : caminho_("") {}
Imagem::Imagem(const std::string& caminho) : caminho_(caminho) {}
std::string Imagem::path() const { return caminho_; }

Retangulo::Retangulo(int x, int y, int largura, int altura)
    : x_(x), y_(y), largura_(largura), altura_(altura) {}
int Retangulo::x() const { return x_; }
int Retangulo::y() const { return y_; }
int Retangulo::largura() const { return largura_; }
int Retangulo::altura() const { return altura_; }

Contagem::Contagem() : imagem_(), caixas_(), segundos_(0.0) {}

Contagem::Contagem(const Imagem& imagem, const Colecao<Retangulo>& caixas,
                   double segundos)
    : imagem_(imagem), caixas_(caixas), segundos_(segundos) {}

int Contagem::total() const { return caixas_.size(); }
double Contagem::processingSeconds() const { return segundos_; }
Colecao<Retangulo> Contagem::boxes() const { return caixas_; }
Imagem Contagem::image() const { return imagem_; }

} // namespace contagem_gado
