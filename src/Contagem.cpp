#include "Contagem.hpp"

Contagem::Contagem() : foto(), marcacoes(), segundos(0.0) {}

Contagem::Contagem(const Imagem& imagem, const Colecao<Retangulo>& caixas, double segundos)
    : foto(imagem), marcacoes(caixas), segundos(segundos) {}

int Contagem::total() const { return marcacoes.size(); }

double Contagem::processingSeconds() const { return segundos; }

Colecao<Retangulo> Contagem::boxes() const { return marcacoes; }

Imagem Contagem::image() const { return foto; }
