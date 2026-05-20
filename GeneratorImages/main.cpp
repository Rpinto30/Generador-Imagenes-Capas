#include <iostream>
#include "structs.h"
#include "DotGenerator.h"
#include "definitions.h"

using namespace std;
using namespace definitions;
using namespace dotGenerator;



BST<User> bt("Usuarios", "usr_", "red");
BST<Layer_struct> bt_layers("Capas", "lyr_", "\"#FF333340\"");
ListImages ls("Imagenes", "img_", "\"#7FBA85\"");

DotFile dot_file("User_ex", "us_ej", "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");

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
    //bt.preOrden_graphviz();
    bt.print_preorden();

    Layer* l1 = new Layer;
    Layer* l2 = new Layer;

    bt_layers.insert(10, new Layer_struct("l0", l1));
    bt_layers.insert(5, new Layer_struct("15", l2));
    bt_layers.insert(4, new Layer_struct("14", l1));
    bt_layers.insert(15, new Layer_struct("115", l2));
    bt_layers.insert(9, new Layer_struct("19", l1));
    bt_layers.insert(20, new Layer_struct("120", l1));

    Image* img = new Image;
    ls.add(1, img);
    ls.add(2, img);
    ls.add(4, img);
    ls.add(5, img);

    generateDot(true);
    cout<<"Termino";
    return 0;
}

void graphIntoDotFile(){
    SubgraphQueue* dot_queue = dot_file.getQueue();
    dot_queue->add(bt.getGraph());
    dot_queue->add(ls.getGraph());
    dot_queue->add(bt_layers.getGraph());
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
