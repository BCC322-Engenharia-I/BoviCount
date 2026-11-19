#include "core/Data.hpp"

Data::Data() : ano(0), mes(0), dia(0) {}

Data::Data(int ano, int mes, int dia) : ano(ano), mes(mes), dia(dia) {}

int Data::year() const { return ano; }

int Data::month() const { return mes; }

int Data::day() const { return dia; }

bool Data::operator==(const Data& outra) const {
    return ano == outra.ano && mes == outra.mes && dia == outra.dia;
}

bool Data::operator<(const Data& outra) const {
    if (ano != outra.ano) return ano < outra.ano;
    if (mes != outra.mes) return mes < outra.mes;
    return dia < outra.dia;
}
