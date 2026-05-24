#ifndef H_AUTOMATA_CAP
#define H_AUTOMATA_CAP
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <regex>
#include "structs.h"
using namespace std;
using namespace st;

namespace auto_cap {

typedef struct DataCap {
    int row;
    int column;
    string color;
} DataCap;

typedef struct BlockCap {
    string id;
    LinkedList<DataCap>* rows;
} BlockCap;

LinkedList<string> tokenizar(const string &content) {
    LinkedList<string> tokens;
    regex re("[#a-zA-Z0-9_]+|[{};,]");
    auto init = sregex_iterator(content.begin(), content.end(), re);
    auto fin  = sregex_iterator();
    for (auto it = init; it != fin; ++it)
        tokens.insert_node((*it).str());
    return tokens;
}

LinkedList<BlockCap> parseCap(const string &content) {
    enum STATE { INIT, WAIT_BRACKET, IN_BLOCK };

    LinkedList<string>  tokens = tokenizar(content);
    LinkedList<BlockCap> blocks;
    BlockCap actual;
    STATE state = INIT;

    auto isAlphanumeric = [](const string &t) {
        return regex_match(t, regex("[a-zA-Z0-9_#]+"));
    };

    auto it  = tokens.begin();
    auto end = tokens.end();

    while (it != end) {
        const string token = *it;

        switch (state) {

        case INIT:
            if (isAlphanumeric(token)) {
                actual.id   = token;
                actual.rows = new LinkedList<DataCap>();
                state = WAIT_BRACKET;
            } else {
                throw runtime_error("Token inesperado en INIT: " + token);
            }
            break;

        case WAIT_BRACKET:
            if (token == "{")
                state = IN_BLOCK;
            else
                throw runtime_error("Se esperaba '{' pero se encontró: " + token);
            break;

        case IN_BLOCK:
            if (token == "}") {
                blocks.insert_node(actual);
                state = INIT;

            } else if (isAlphanumeric(token)) {
                DataCap row;
                row.row = stoi(token);

                ++it; if (it == end || *it != ",") throw runtime_error("Se esperaba ',' tras fila");
                ++it; if (it == end)               throw runtime_error("Falta columna");
                row.column = stoi(*it);

                ++it; if (it == end || *it != ",") throw runtime_error("Se esperaba ',' tras columna");
                ++it; if (it == end)               throw runtime_error("Falta color");
                row.color = *it;

                ++it; if (it == end || *it != ";") throw runtime_error("Se esperaba ';'");

                actual.rows->insert_node(row);

            } else {
                throw runtime_error("Token inesperado en IN_BLOCK: " + token);
            }
            break;
        }
        ++it;
    }

    if (state != INIT)
        throw runtime_error("Archivo incompleto: bloque sin cerrar '}'");

    return blocks;
}

LinkedList<BlockCap> loadCap(const string &filepath) {
    ifstream file(filepath);
    if (!file.is_open())
        throw runtime_error("No se pudo abrir el archivo: " + filepath);

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return parseCap(buffer.str());
}

} // namespace auto_cap
#endif // H_AUTOMATA_CAP
