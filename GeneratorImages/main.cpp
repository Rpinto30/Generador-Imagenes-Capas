#include <iostream>
#include <chrono> //lo unico donde hecho de menos python, el time xd

#include "structs.h"
#include "DotGenerator.h"
#include "definitions.h"
#include "automata_cap.h"
#include "automata_im.h"
#include "automata_usr.h"


using namespace std;
using namespace definitions;
using namespace dotGenerator;
using namespace auto_cap;
using namespace auto_im;
using namespace auto_usr;

int last_id_user = 1;
BST<User> bt("Usuarios", "us_", "white");
BST<Layer_struct> bt_layers("Capas", "lyr_", "\"#FF333340\"");
ListImages ls("Imagenes", "img_", "\"#7FBA85\"", "lyr_");


DotFile dot_file("GESTION_VISUAL_DE_MEMORIA", "output", "\nrankdir=\"LR\"; nodesep=0.5; ranksep=0.8; splines=ortho;\n");

/*REFERENCIA A FUNCIONES*/
void graphIntoDotFile();
int generateDot(bool debugMessage);

void load_layers();
void load_images();
void load_users();

int massive(bool message = true){
    //graphIntoDotFile();
     if (message) cout<<"Cargado archivos..."<<endl;
    try{
        load_layers();
        load_images();
        load_users();
        if (message) cout<<"! Archivos cargados!"<<endl;
        return 0;
    } catch(const runtime_error& e){
        if (message) cout<<"Hubo un error al cargar los archivos... porfavor, reincia el sistema..."<<endl;
        return -1;
    }


}


/*METODOS FRONT (DE INTERFAZ PARA LOS CHAVOS)*/
void userOptions(){
    string option;
    do{
        cout<<"-------------------------------------------------------------------------"<<endl;
        cout<<"\t1) Crear un nuevo usuario"<<endl;
        cout<<"\t2) Ver Usuarios"<<endl;
        cout<<"\t3) Eliminar un usuario"<<endl;
        cout<<"\t4) Modificar un usuario"<<endl;
        cout<<"\t-1) Volver"<<endl;
        cout<<"\t> Elige una de las opciones: ";
        cin>>option;

        if (option == "1"){
            string name;
            cout<<"\t---------------------------------"<<endl;
            cout<<"\t> Agrega un nombre al usuario: ";
            cin>>name;
            User* user_ = new User("us_" + to_string(last_id_user));
            user_->nickname = name;
            bt.insert(last_id_user, user_, user_->nickname);
            cout<<"\t! Se ha creado el usuario y agregado al sistema!"<<endl; break;
        }

    } while(option != "-1");
}

void imagesOptions(){
    string option;
    do{
        cout<<"-------------------------------------------------------------------------"<<endl;
        cout<<"\t1) Generar por usuario"<<endl;
        cout<<"\t2) Generar por capas"<<endl;
        cout<<"\t3) Generar por recorrido"<<endl;
        cout<<"\t-1) Volver"<<endl;
        cout<<"\t> Elige una de las opciones: ";
        cin>>option;
    } while(option != "-1");
}

void memoryOptions(){
    string option;
    do{
        cout<<"-------------------------------------------------------------------------"<<endl;
        cout<<"\t1) Mostrar Imagenes"<<endl;
        cout<<"\t2) Mostrar Arbol de capas"<<endl;
        cout<<"\t3) Mostrar capa"<<endl;
        cout<<"\t4) Mostrar Imagen y arbol de capas"<<endl;
        cout<<"\t5) Mostrar Arbol de usuarios"<<endl;
        cout<<"\t-1) Volver"<<endl;
        cout<<"\t> Elige una de las opciones: ";
        cin>>option;
    } while(option != "-1");
}



int main()
{
    if(massive() != 0) return -1;
    string option;
    do{
        cout<<endl<<"=========================GENERADOR DE IMAGENES EN PIXEL-ART========================="<<endl;
        cout<<"1) Opciones de usuario"<<endl;
        cout<<"2) Generacion de imagenes"<<endl;
        cout<<"3) Gestionar la memoria"<<endl;
        cout<<"-1) Salir"<<endl;
        cout<<"> Elige una de las opciones: ";
        cin>>option;

        if (option == "1") userOptions();
        if (option == "2") imagesOptions();
        if (option == "3") memoryOptions();
    } while(option != "-1");

    //generateDot(true);
    return 0;
}

int generateDotMatrix(string tittle, SubGraph* subgraph, bool debugMessage = true){
    DotFile temp_file(tittle,  tittle, "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");
    temp_file.getQueue()->add(subgraph);
    cout<<"Generando matriz de Layer: " + tittle;
    int t = temp_file.generateNewFiles();

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

void drawPixelArt(string tittle, Layer* layer, bool debugMessage = true){
    PixelGraph pixel_generator(tittle, "pixelArt_" + tittle);
    if(debugMessage = true) cout<<"Creando Pixel art..."<<endl;
    pixel_generator.addToContext(layer->pixelArt(18,18));
    pixel_generator.generateNewFiles();
    if (debugMessage = true) cout<<"PixelArt Creado!"<<endl;
}

void load_layers(){
    try {
        LinkedList<BlockCap> bloques = loadCap("capas.cap");
        for (BlockCap& bloque : bloques) {
            Layer_struct* layer = new Layer_struct(new Layer("l"+ bloque.id));
            for (DataCap& fila : *(bloque.rows)) {
                layer->layer->insert(fila.row, fila.column, fila.color);
            }
            //drawPixelArt("layer_"+bloque.id, layer->layer);
            //generateDotMatrix("layer_"+bloque.id, layer->layer->getGraph());
            bt_layers.insert(stoi(bloque.id), layer, "");
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
}

void load_images(){
     try {
        LinkedList<BlockIm> bloques = loadIm("imagenes.im");
        for (BlockIm& bloque : bloques) {
            Image* image = new Image(bloque.id);
            ls.add(stoi(bloque.id), image);
            for (string& capa : *(bloque.layers)){
                ls.addLayer(stoi(bloque.id), bt_layers.search(stoi(capa))->data);
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
}

void load_users(){
    try {
        LinkedList<UserUsr> users = loadUsr("usuarios.usr");
        int id = 1;
        for (UserUsr& user : users) {
            User* user_ = new User("us_" + to_string(id));
            user_->nickname = user.name;
            bt.insert(id, user_, user_->nickname);
            for (string& img : *(user.images)){
                Image* img_ = ls.getByID(stoi(img));
                if (!img_) continue;
                user_->list_images.add(stoi(img), img_, bt.getGraph());

            }
            id++;
        }
        last_id_user = id+1;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
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
