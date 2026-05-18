#include <iostream>
#include "structs.h"
#include "DotGenerator.h"
#include "definitions.h"

using namespace std;
using namespace definitions;
using namespace dotGenerator;



BST<User> bt("Usuarios", "usr_", "black");
DotFile dot_file("User_ex", "us_ej");

/*REFERENCIA A FUNCIONES*/
void graphIntoDotFile();
int generateDot(bool debugMessage);

int main()
{
    graphIntoDotFile();
    bt.insert(10 , new User("pene"));
    bt.insert(15 , new User("pene"));
    bt.insert(5 , new User("pene"));
    bt.insert(3 , new User("pene"));
    bt.insert(12 , new User("pene"));
    bt.insert(8 , new User("pene"));
    bt.insert(17, new User("pene"));
    bt.insert(16, new User("pene"));
    bt.preOrden_graphviz();
    bt.print_preorden();
    generateDot(true);
    return 0;
}

void graphIntoDotFile(){
    SubgraphQueue* dot_queue = dot_file.getQueue();
    dot_queue->add(bt.getGraph());
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


// Graphviz
// Mermaid.js
