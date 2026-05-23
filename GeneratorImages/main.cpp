#include <iostream>
#include "structs.h"
#include "DotGenerator.h"
#include "definitions.h"

using namespace std;
using namespace definitions;
using namespace dotGenerator;



BST<User> bt("Usuarios", "us_", "red");
BST<Layer_struct> bt_layers("Capas", "lyr_", "\"#FF333340\"");
ListImages ls("Imagenes", "img_", "\"#7FBA85\"", "lyr_");


DotFile dot_file("GESTION_VISUAL_DE_MEMORIA", "us_ej", "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");

/*REFERENCIA A FUNCIONES*/
void graphIntoDotFile();
int generateDot(bool debugMessage);

int main()
{
    graphIntoDotFile();

    User* us = new User("us_10");
    bt.insert(10, us);
    User* us2 = new User("us_5");
    bt.insert(5, us2);
    User* us3 = new User("us_6");
    bt.insert(6, us3);

    /*bt.insert(15 , new User);
    bt.insert(5 , new User);
    bt.insert(3 , new User);
    bt.insert(12 , new User);
    bt.insert(8 , new User);
    bt.insert(17, new User);*/
    //bt.preOrden_graphviz();

    Layer* l1 = new Layer("l1");
    Layer* l2 = new Layer("l2");
    Layer* l3 = new Layer("l3");
    Layer* l4 = new Layer("l4");
    Layer* l5 = new Layer("l5");
    Layer* l6 = new Layer("l6");
    Layer* l7 = new Layer("l7");
    dot_file.getQueue()->add(l1->getGraph());
    l1->insert(1, 1, "#asdqwe");
    l1->insert(2, 4, "#zxcasd");
    l1->insert(3, 5, "#cvbdfg");
    l1->insert(6, 6, "#345fdg");

    bt_layers.insert(9, new Layer_struct(l1));
    bt_layers.insert(2, new Layer_struct(l2));
    bt_layers.insert(5, new Layer_struct(l3));
    bt_layers.insert(11, new Layer_struct(l4));
    bt_layers.insert(1, new Layer_struct(l5));
    bt_layers.insert(55, new Layer_struct(l6));
    bt_layers.insert(10, new Layer_struct(l7));

    Image* img = new Image("1");
    Image* img2 = new Image("2");
    Image* img3 = new Image("3");
    Image* img4 = new Image("4");
    ls.add(15, img);
    ls.add(20, img2);
    ls.add(25, img3);
    ls.add(30, img4);
    ls.addLayer(20, bt_layers.search(11)->data);
    ls.addLayer(20, bt_layers.search(2)->data);
    ls.addLayer(15, bt_layers.search(5)->data);
    ls.addLayer(20, bt_layers.search(10)->data);

    us->list_images.add(15, img, bt.getGraph());
    us->list_images.add(25, img, bt.getGraph());

    us2->list_images.add(25, img, bt.getGraph());


    //User* us4 = new User("us_15");
    //bt.insert(15, us4);

    generateDot(false);
    return 0;
}

void graphIntoDotFile(){
    SubgraphQueue* dot_queue = dot_file.getQueue();
    dot_queue->add(bt_layers.getGraph());
    dot_queue->add(ls.getGraph());
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
