#ifndef CONTAGEM_HPP
#define CONTAGEM_HPP

#include "core/Colecao.hpp"
#include "contagem/Imagem.hpp"
#include "contagem/Retangulo.hpp"

// O resultado da analise de uma foto. Ainda NAO e o historico: so vira
// historico quando o usuario confirma e salva.
class Contagem {
public:
    Contagem();
    Contagem(const Imagem& imagem, const Colecao<Retangulo>& caixas, double segundos);

    int total() const;                  // total de animais encontrados
    double processingSeconds() const;   // tempo gasto na analise, em segundos
    Colecao<Retangulo> boxes() const;   // uma marcacao para cada animal
    Imagem image() const;               // a foto que foi analisada

private:
    Imagem foto;
    Colecao<Retangulo> marcacoes;
    double segundos;
};

#endif
