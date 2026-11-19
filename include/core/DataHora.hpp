#ifndef DATAHORA_HPP
#define DATAHORA_HPP

#include "core/Data.hpp"

// Uma data com hora; registra o momento exato em que uma contagem foi salva.
class DataHora {
public:
    DataHora();
    DataHora(const Data& data, int hora, int minuto, int segundo);

    // Momento atual, segundo o relogio do computador.
    static DataHora agora();

    Data date() const;
    int hour() const;
    int minute() const;
    int second() const;

private:
    Data data;
    int hora;
    int minuto;
    int segundo;
};

#endif
