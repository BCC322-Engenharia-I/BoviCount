#include "DataHora.hpp"
#include <ctime>

DataHora::DataHora() : data(), hora(0), minuto(0), segundo(0) {}

DataHora::DataHora(const Data& data, int hora, int minuto, int segundo)
    : data(data), hora(hora), minuto(minuto), segundo(segundo) {}

DataHora DataHora::agora() {
    std::time_t t = std::time(0);
    std::tm* p = std::localtime(&t);
    if (p == 0) {
        return DataHora();
    }
    return DataHora(Data(p->tm_year + 1900, p->tm_mon + 1, p->tm_mday),
                    p->tm_hour, p->tm_min, p->tm_sec);
}

Data DataHora::date() const { return data; }

int DataHora::hour() const { return hora; }

int DataHora::minute() const { return minuto; }

int DataHora::second() const { return segundo; }
