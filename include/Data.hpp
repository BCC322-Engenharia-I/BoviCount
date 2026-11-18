#ifndef DATA_HPP
#define DATA_HPP

// Uma data, por exemplo 03/10/2026.
class Data {
public:
    Data();
    Data(int ano, int mes, int dia);

    int year() const;
    int month() const;
    int day() const;

    bool operator==(const Data& outra) const;
    bool operator<(const Data& outra) const;

private:
    int ano;
    int mes;
    int dia;
};

#endif
