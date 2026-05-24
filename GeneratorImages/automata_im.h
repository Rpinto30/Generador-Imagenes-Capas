#ifndef H_AUTOMATA_IM
#define H_AUTOMATA_IM
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <regex>
#include "structs.h"
using namespace std;
using namespace st;

namespace auto_im {

typedef struct BlockIm {
    string id;
    LinkedList<string>* layers;
} BlockIm;


LinkedList<string> tokenizar(const string &content) {
    LinkedList<string> tokens;
    regex re("[a-zA-Z0-9_#]+|[{},]");
    auto init = sregex_iterator(content.begin(), content.end(), re);
    auto fin  = sregex_iterator();
    for (auto it = init; it != fin; ++it)
        tokens.insert_node((*it).str());
    return tokens;
}


LinkedList<BlockIm> parseIm(const string &content) {
    enum STATE { INIT, WAIT_BRACKET, IN_BLOCK };

    LinkedList<string>  tokens = tokenizar(content);
    LinkedList<BlockIm> blocks;
    BlockIm actual;
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
                actual.id     = token;
                actual.layers = new LinkedList<string>();
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

            } else if (token == ",") {

            } else if (isAlphanumeric(token)) {
                actual.layers->insert_node(token);

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

LinkedList<BlockIm> loadIm(const string &filepath) {
    ifstream file(filepath);
    if (!file.is_open())
        throw runtime_error("No se pudo abrir el archivo: " + filepath);

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return parseIm(buffer.str());
}

} // namespace auto_im
#endif // H_AUTOMATA_IM
