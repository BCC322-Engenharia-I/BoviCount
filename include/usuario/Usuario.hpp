#ifndef USUARIO_H
#define USUARIO_H

#include <string>

// Uma pessoa que usa o sistema.
class Usuario {
private:
    int idUsuario;
    std::string papel;
    int idPropriedade;

public:
    Usuario();
    Usuario(int id, const std::string& papel, int idPropriedade);

    int id() const;               // numero que identifica o usuario
    std::string role() const;     // "Gestor", "Funcionário" ou "Administrador"
    int propertyId() const;       // fazenda a que o usuario pertence
};

#endif // USUARIO_H