#include <iostream>
#include "structs.h"
#include "DotGenerator.h"
#include "definitions.h"
#include "automata_cap.h"

using namespace std;
using namespace definitions;
using namespace dotGenerator;
using namespace auto_cap;

BST<User> bt("Usuarios", "us_", "red");
BST<Layer_struct> bt_layers("Capas", "lyr_", "\"#FF333340\"");
ListImages ls("Imagenes", "img_", "\"#7FBA85\"", "lyr_");


DotFile dot_file("GESTION_VISUAL_DE_MEMORIA", "us_ej", "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");

/*REFERENCIA A FUNCIONES*/
void graphIntoDotFile();
int generateDot(bool debugMessage);

void corazoncit(){
    Layer* l1 = new Layer("l1");
    l1->insert(1, 2, "#FF0000");
    l1->insert(1, 4, "#FF0000");
    l1->insert(2, 1, "#FF0000");
    l1->insert(2, 2, "#FF0000");
    l1->insert(2, 3, "#FF0000");
    l1->insert(2, 4, "#FF0000");
    l1->insert(2, 5, "#FF0000");
    l1->insert(3, 1, "#FF0000");
    l1->insert(3, 2, "#FF0000");
    l1->insert(3, 3, "#FF0000");
    l1->insert(3, 4, "#FF0000");
    l1->insert(3, 5, "#FF0000");
    l1->insert(4, 2, "#FF0000");
    l1->insert(4, 3, "#FF0000");
    l1->insert(4, 4, "#FF0000");
    l1->insert(5, 3, "#FF0000");
    l1->pixelArt();
    delete l1;
}

int main()
{
     try {
        LinkedList<BlockCap> bloques = loadCap("capas.cap");
        cout<<"leido";
        for (BlockCap& bloque : bloques) {
            cout << "ID: " << bloque.id << "\n";
            for (DataCap& fila : *(bloque.rows)) {
                cout << "  fila="  << fila.row
                     << " col="   << fila.column
                     << " color=" << fila.color << "\n";
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
    return 0;

    //graphIntoDotFile();
    /*
    User* us = new User("us_10");
    bt.insert(10, us);
    User* us2 = new User("us_5");
    bt.insert(5, us2);
    User* us3 = new User("us_6");
    bt.insert(6, us3);

    Layer* l1 = new Layer("l1");
    Layer* l2 = new Layer("l2");
    Layer* l3 = new Layer("l3");
    Layer* l4 = new Layer("l4");
    Layer* l5 = new Layer("l5");
    Layer* l6 = new Layer("l6");
    Layer* l7 = new Layer("l7");
    //Crear archivo de imagen_memoria
    dot_file.getQueue()->add(l1->getGraph());
    //dot_file.getQueue()->add(l2->getGraph());

    l1->insert(1,7,"#E88E5D");
    l1->insert(1,8,"#E88E5D");
    for (int i = 2; i<= 6; i++){
        l1->insert(i,6,"#E88E5D");
        l1->insert(i,7,"#F1CCBC");
        l1->insert(i,8,"#F1CCBC");
        l1->insert(i,9,"#E88E5D");
    }
    l1->insert(7,4,"#E88E5D");
    l1->insert(7,5,"#E88E5D");
    l1->insert(7,6,"#E88E5D");
    l1->insert(7,7,"#F1CCBC");
    l1->insert(7,8,"#F1CCBC");
    l1->insert(7,9,"#E88E5D");
    l1->insert(8,3,"#E88E5D");
    l1->insert(8,4,"#F1CCBC"); //luz
    l1->insert(8,5,"#F1CCBC"); //luz
    l1->insert(8,6,"#E88E5D");
    l1->insert(8,7,"#F1CCBC");
    l1->insert(8,8,"#F1CCBC");
    l1->insert(8,9,"#E88E5D");
    l1->insert(8,10,"#E88E5D");
    l1->insert(8,11,"#E88E5D");

    l1->insert(9,3,"#E88E5D");
    l1->insert(9,4,"#F1CCBC");
    l1->insert(9,5,"#F1CCBC");
    l1->insert(9,6,"#E88E5D"); //sombra 2
    l1->insert(9,7,"#F1CCBC");
    l1->insert(9,8,"#F1CCBC");
    l1->insert(9,9,"#E88E5D");
    l1->insert(9,10,"#F1CCBC"); //luz
    l1->insert(9,11,"#F1CCBC"); //luz
    l1->insert(9,12,"#E88E5D");
    l1->insert(9,13,"#E88E5D");
    l1->insert(9,14,"#E88E5D");

    l1->insert(10,3,"#E88E5D");
    l1->insert(10,4,"#F1CCBC");
    l1->insert(10,5,"#F1CCBC");
    l1->insert(10,6,"#F1CCBC");
    l1->insert(10,7,"#F1CCBC");
    l1->insert(10,8,"#F1CCBC");
    l1->insert(10,9,"#F1CCBC");
    l1->insert(10,10,"#F1CCBC"); //luz
    l1->insert(10,11,"#F1CCBC"); //luz
    l1->insert(10,12,"#E88E5D");
    l1->insert(10,13,"#F1CCBC");
    l1->insert(10,14,"#F1CCBC");
    l1->insert(10,15,"#E88E5D");

    l1->insert(11,2,"#E88E5D");
    l1->insert(11,3,"#F1CCBC");
    l1->insert(11,4,"#F1CCBC");
    l1->insert(11,5,"#F1CCBC");
    l1->insert(11,6,"#F1CCBC");
    l1->insert(11,7,"#F1CCBC");
    l1->insert(11,8,"#F1CCBC");
    l1->insert(11,9,"#F1CCBC");
    l1->insert(11,10,"#F1CCBC"); //luz
    l1->insert(11,11,"#F1CCBC"); //luz
    l1->insert(11,12,"#F1CCBC");
    l1->insert(11,13,"#F1CCBC");
    l1->insert(11,14,"#F1CCBC");
    l1->insert(11,15,"#E88E5D");


    for (int i = 12; i <= 14; i++){
        l1->insert(i,1,"#E88E5D");
        l1->insert(i,2,"#F1CCBC");
        l1->insert(i,3,"#F1CCBC");
        l1->insert(i,4,"#F1CCBC");
        l1->insert(i,5,"#F1CCBC");
        l1->insert(i,6,"#F1CCBC");
        l1->insert(i,7,"#F1CCBC");
        l1->insert(i,8,"#F1CCBC");
        l1->insert(i,9,"#F1CCBC");
        l1->insert(i,10,"#F1CCBC");
        l1->insert(i,11,"#F1CCBC");
        l1->insert(i,12,"#F1CCBC");
        l1->insert(i,13,"#F1CCBC");
        l1->insert(i,14,"#F1CCBC");
        l1->insert(i,15,"#E88E5D");
    }

    for (int i = 11; i<=15; i+=4){
        l1->insert(i,2,"#E88E5D");
        l1->insert(i,3,"#F1CCBC");
        l1->insert(i,4,"#F1CCBC");
        l1->insert(i,5,"#F1CCBC");
        l1->insert(i,6,"#F1CCBC");
        l1->insert(i,7,"#F1CCBC");
        l1->insert(i,8,"#F1CCBC");
        l1->insert(i,9,"#F1CCBC");
        l1->insert(i,10,"#F1CCBC");
        l1->insert(i,11,"#F1CCBC");
        l1->insert(i,12,"#F1CCBC");
        l1->insert(i,13,"#F1CCBC");
        l1->insert(i,14,"#F1CCBC");
        l1->insert(i,15,"#E88E5D");
    }

    for (int i = 16; i<=18; i++){
        l1->insert(i,3,"#E88E5D");
        l1->insert(i,4,"#F1CCBC");
        l1->insert(i,5,"#F1CCBC");
        l1->insert(i,6,"#F1CCBC");
        l1->insert(i,7,"#F1CCBC");
        l1->insert(i,8,"#F1CCBC");
        l1->insert(i,9,"#F1CCBC");
        l1->insert(i,10,"#F1CCBC");
        l1->insert(i,11,"#F1CCBC");
        l1->insert(i,12,"#F1CCBC");
        l1->insert(i,13,"#F1CCBC");
        l1->insert(i,14,"#E88E5D");
    }

    l2->insert(2, 7, "#F5DFC4");
    l2->insert(2, 8, "#F5DFC4");

    l2->insert(8, 4, "#F5DFC4");
    l2->insert(8, 5, "#F5DFC4");

    l2->insert(9, 10, "#F5DFC4");
    l2->insert(9, 11, "#F5DFC4");

    l2->insert(10, 13, "#F5DFC4");
    l2->insert(10, 14, "#F5DFC4");

    l2->insert(11, 3, "#F5DFC4");
    l2->insert(11, 4, "#F5DFC4");
    l2->insert(11, 5, "#F5DFC4");


    l2->insert(12, 2, "#F5DFC4");


    l3->insert(9, 6, "#EBA36C");
    l3->insert(10, 4, "#EBA36C");
    l3->insert(10, 5, "#EBA36C");
    l3->insert(11, 12, "#EBA36C");
    l3->insert(10, 9, "#EBA36C");
    l3->insert(11, 6, "#EBA36C");
    l3->insert(11, 9, "#EBA36C");
    l3->insert(12, 6, "#EBA36C");
    l3->insert(12, 10, "#EBA36C");
    l3->insert(12, 11, "#EBA36C");
    l3->insert(12, 12, "#EBA36C");


    l3->insert(13, 4, "#EBA36C");
    l3->insert(13, 5, "#EBA36C");
    l3->insert(13, 13, "#EBA36C");
    l3->insert(13, 12, "#EBA36C");


    l3->insert(14, 3, "#EBA36C");
    l3->insert(16, 2, "#EBA36C");


    l3->insert(17, 3, "#EBA36C");
    l3->insert(17, 14, "#EBA36C");


    l3->insert(18, 3, "#EBA36C");
    l3->insert(18, 14, "#EBA36C");

    PixelGraph px_l1;
    cout<<"Creando Pixel art..."<<endl;
    px_l1.addToContext(l1->pixelArt(18,18));
    px_l1.addToContext(l2->pixelArt(18,18));
    px_l1.addToContext(l3->pixelArt(18,18));
    px_l1.generateNewFiles();

    cout<<"PixelArt Creado!"<<endl;

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

    generateDot(false);*/
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
