#ifndef H_AUTOMATA_USR
#define H_AUTOMATA_USR
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <regex>
#include "structs.h"
using namespace std;
using namespace st;

namespace auto_usr {

typedef struct UserUsr {
    string name;
    LinkedList<string>* images = nullptr;
} UserUsr;


LinkedList<string> tokenizar(const string &content) {
    LinkedList<string> tokens;
    regex re("[a-zA-Z0-9_]+|[:,;]");
    auto init = sregex_iterator(content.begin(), content.end(), re);
    auto fin  = sregex_iterator();
    for (auto it = init; it != fin; ++it)
        tokens.insert_node((*it).str());
    return tokens;
}

LinkedList<UserUsr> parseUsr(const string &content) {
    enum STATE { INIT, WAIT_COLON, IN_LIST };

    LinkedList<string>  tokens = tokenizar(content);
    LinkedList<UserUsr> users;
    UserUsr actual;
    STATE state = INIT;

    auto isAlphanumeric = [](const string &t) {
        return regex_match(t, regex("[a-zA-Z0-9_]+"));
    };

    auto it  = tokens.begin();
    auto end = tokens.end();

    while (it != end) {
        const string token = *it;

        switch (state) {
        case INIT:
            if (isAlphanumeric(token)) {
                actual.name   = token;
                actual.images = new LinkedList<string>();
                state = WAIT_COLON;
            } else {
                throw runtime_error("Token inesperado en INIT: " + token);
            }
            break;
        case WAIT_COLON:
            if (token == ":")
                state = IN_LIST;
            else
                throw runtime_error("Se esperaba ':' pero se encontró: " + token);
            break;

        case IN_LIST:
            if (token == ";") {
                users.insert_node(actual);
                actual = UserUsr();
                state = INIT;

            } else if (token == ",") {

            } else if (isAlphanumeric(token)) {
                actual.images->insert_node(token);

            } else {
                throw runtime_error("Token inesperado en IN_LIST: " + token);
            }
            break;
        }

        ++it;
    }

    if (state != INIT)
        throw runtime_error("Archivo incompleto: usuario sin cerrar ';'");

    return users;
}

LinkedList<UserUsr> loadUsr(const string &filepath) {
    ifstream file(filepath);
    if (!file.is_open())
        throw runtime_error("No se pudo abrir el archivo: " + filepath);

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return parseUsr(buffer.str());
}

}
#endif // H_AUTOMATA_USR
