#include "contagem/Imagem.hpp"

Imagem::Imagem() : caminho() {}

Imagem::Imagem(const std::string& caminho) : caminho(caminho) {}

std::string Imagem::path() const { return caminho; }
