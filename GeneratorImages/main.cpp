#include <iostream>
#include "structs.h"
#include "DotGenerator.h"

using namespace std;
using namespace st;
using namespace dotGenerator;

DotFile dot_file;

void graphIntoDotFile(){ // Este metodo me ayuda a insertar todos los subgraphs de las EDD que voy a querer visualizar
    SubgraphQueue* dot_queue = dot_file.getQueue();
}

int generateDot(bool debugMessage = false){
    int t = dot_file.generateNewFiles();
    if (t == 0){
        if(debugMessage) cout<<" $ .dot generador con exito"<<endl;
        return 0;
    } else if (t == 2) {
         if(debugMessage) cout<<" $ .ya generado"<<endl;
         return 1;
    } else{
        if(debugMessage) cout<<" x Error al crear el .dot"<<endl;
        return -1;
    }
}

int main()
{
    BTS<User> bt;
    bt.insert(10 , new User("pene"));
    bt.insert(15 , new User("pene"));
    bt.insert(5 , new User("pene"));
    bt.insert(3 , new User("pene"));
    bt.insert(12 , new User("pene"));
    bt.insert(8 , new User("pene"));
    bt.insert(17, new User("pene"));

    bt.print_bts();

    graphIntoDotFile();
    generateDot();


    return 0;
}


// Graphviz
// Mermaid.js
