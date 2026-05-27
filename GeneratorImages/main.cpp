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

void genImageByUser(int id);
void genImageByLayer(int id);
void generateBySearch();

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
        cout<<"\t5) Ver imagenes de usuario"<<endl;
        cout<<"\t6) Agregar imagen a usuario"<<endl;
        cout<<"\t7) Eliminar imagen de usuario"<<endl;
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
        } else if (option == "2"){
             cout<<"\t---------------------------------"<<endl;
             bt.print_preorden("\t", "Usuario");
        } else if (option == "3"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            bt.print_preorden("\t", "Usuario");
            cout<<"\t> Selecciona el id a eliminar: ";
            cin>>id;
            bt.delete_fromBST(id, "\t");
        } else if (option == "4"){
            int id;
            string nick;
            cout<<"\t---------------------------------"<<endl;
            bt.print_preorden("\t", "Usuario");
            cout<<"\t> Selecciona el id a modificar: ";
            cin>>id;
            cout<<"\t> Nuevo nombre de usuario: ";
            cin>>nick;
            bt.updateName(id, nick, "\t");
        }
        else if (option == "5"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            bt.print_preorden("\t", "Usuario");
            cout<<"\t> Selecciona el id del usuario: ";
            cin>>id;
            Entity<User>* user_ = bt.search(id);
            if (!user_) continue;
            user_->data->list_images.printData("\t");

        }else if (option == "6"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            bt.print_preorden("\t", "Usuario");
            cout<<"\t> Selecciona el id del usuario: ";
            cin>>id;
            Entity<User>* user_ = bt.search(id);
            if (!user_) continue;
            int id_img;
            cout<<"\t Imagenes del usuario: "<<endl;
            user_->data->list_images.printData("\t");
            ls.print_list("\t");
            cout<<"\t> Selecciona el id de la imagen que deseas agregar: ";
            cin>>id_img;
            if (user_->data->list_images.getImageIndex(id_img)){
                cout<<"\tx Esa imagen ya esta utilizada por ese usuario!"<<endl;
                continue;
            } else{
                Image* img = ls.getByID(id_img);
                user_->data->list_images.add(id_img, img, bt.getGraph());
                cout<<"\t! Imagen agregada correctamente al usuario!"<<endl;
            }
        } else if (option == "7"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            bt.print_preorden("\t", "Usuario");
            cout<<"\t> Selecciona el id del usuario: ";
            cin>>id;
            Entity<User>* user_ = bt.search(id);
            if (!user_) continue;
            int id_img;
            cout<<"\t Imagenes del usuario: "<<endl;
            user_->data->list_images.printData("\t");
            cout<<"\t> Selecciona el id de la imagen que deseas remover: ";
            cin>>id_img;
            if (user_->data->list_images.getImageIndex(id_img)){
                user_->data->list_images.delete_id(id_img);
                cout<<"\t! La imagen fue removida del usuario con exito!"<<endl;
            } else{
                cout<<"\tx Esa imagen no es propia del usuario, no se puede remover!"<<endl;
                continue;
            }
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

        if (option == "1"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            bt.print_preorden("\t", "Usuario");
            cout<<"\t> Selecciona el id del usuario: ";
            cin>>id;
            genImageByUser(id);
        } else if (option == "2"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            bt_layers.print_preorden("\t", "");
            cout<<"\t> Selecciona el id del usuario: ";
            cin>>id;
            genImageByLayer(id);
        } else if (option == "3"){
            generateBySearch();
        }
    } while(option != "-1");
}

void generateDotFile(DotFile* file, bool delete_file, bool debugMessage = true){
    int t = file->generateNewFiles(delete_file);
    if (t == 0){
        if(debugMessage) cout<<" ! .dot generador con exito"<<endl;

    } else if (t == 2) {
         if(debugMessage) cout<<" ! .ya generado"<<endl;
    } else{
        if(debugMessage) cout<<" x Error al crear el .dot"<<endl;
    }}

void memoryOptions(){
    string option;
    do{
        cout<<"-------------------------------------------------------------------------"<<endl;
        cout<<"\t1) Mostrar Lista Circular Doble de Imagenes"<<endl;
        cout<<"\t2) Mostrar Arbol de capas"<<endl;
        cout<<"\t3) Mostrar capa"<<endl;
        cout<<"\t4) Mostrar Imagen y arbol de capas"<<endl;
        cout<<"\t5) Mostrar Arbol de usuarios"<<endl;
        cout<<"\t-1) Volver"<<endl;
        cout<<"\t> Elige una de las opciones: ";
        cin>>option;

        if (option == "1"){
            DotFile temp_file("Lista_Doble_Circular_Imagenes",  "dlc_memory", "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");
            SubGraph graph_copy = ls.getOnlyGraph();
            temp_file.getQueue()->add(&graph_copy);
            generateDotFile(&temp_file, true);
        } else if (option == "2"){
            DotFile temp_file("Arbol_Capas",  "bst_layers_memory", "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");
            temp_file.getQueue()->add(bt_layers.getGraph());
            generateDotFile(&temp_file, true);
        } else if (option == "3"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            bt_layers.print_preorden("\t");
            cout<<"\t> Selecciona el id de la capa: ";
            cin>>id;

            Entity<Layer_struct>* ent = bt_layers.search(id);
            if (!ent) continue;
            Layer_struct* layer_ = ent->data;

            DotFile temp_file("Capa_"+to_string(id),  "layer_"+to_string(id), "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");
            temp_file.getQueue()->add(layer_->layer->getGraph());
            generateDotFile(&temp_file, true);
        }  else if (option == "4"){
            int id;
            cout<<"\t---------------------------------"<<endl;
            ls.print_list("\t");
            cout<<"\t> Selecciona el id de la imagen: ";
            cin>>id;

            DotFile temp_file("Arbol_Capas_e_Imagen",  "bst_layers_image_memory", "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");
            SubGraph graph_copy = ls.getOnlyImg(id);
            temp_file.getQueue()->add(&graph_copy);
            temp_file.getQueue()->add(bt_layers.getGraph());
            generateDotFile(&temp_file, true);
        } else if (option == "5"){
            DotFile temp_file("Arbol_Usuarios",  "bst_users", "\nrankdir=TB; nodesep=0.5; ranksep=0.8; splines=ortho;\n");
            SubGraph graph_copy = bt.getGraphOnlyRoot();
            temp_file.getQueue()->add(&graph_copy);
            //temp_file.getQueue()->add(bt.getGraph());
            generateDotFile(&temp_file, true);
        }

    } while(option != "-1");
}


/*MAIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIN*/
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

/*METODOS PARA GENERAR IMAGENES*/
void genImageByUser(int id){
    Entity<User>* ent = bt.search(id);
    if (!ent) return;
    User* user = ent->data;
    cout<<"-------------------------------------------------"<<endl;
    user->list_images.printData("\t");
    int id_image;
    cout<<"\t> Ingresa ID de la imagen que quieres dibujar: ";
    cin>>id_image;
    Image* img = user->list_images.getImageIndex(id_image);
    Node<Layer_struct>* temp = img->layers.getHead();

    if (!temp) return;
    PixelGraph pixel_generator("pixel_art_de_"+user->nickname, "pixelArt_" + user->nickname);
    int img_count = 0;

    int maxX = temp->data->layer->getWidth(); int maxY = temp->data->layer->getHeight();
    while(temp){
        cout<<"\tCargando capa "<<to_string(++img_count)<<"..."<<endl;
        if (temp->data->layer->getWidth() > maxX) maxX = temp->data->layer->getWidth();
        if (temp->data->layer->getHeight() > maxX) maxY = temp->data->layer->getHeight();

        pixel_generator.addToContext(temp->data->layer->pixelArt(maxX,maxY));
        temp = temp->next;
    }


    pixel_generator.generateNewFiles(true);
    cout<<"\t\n ! PixelArt de "<<user->nickname<<" ha sido creado en la carpeta ResultImages!"<<endl;
}

void genImageByLayer(int id){
    Entity<Layer_struct>* ent = bt_layers.search(id);
    if (!ent) return;
    Layer_struct* layer_ = ent->data;

    cout<<"\tSe esta generando tu capa..."<<endl;
    PixelGraph pixel_generator("capa_" + to_string(id), "capa_" + to_string(id));
    pixel_generator.addToContext(layer_->layer->pixelArt());
    pixel_generator.generateNewFiles(true);
    cout<<"\t\n ! PixelArt ha sido creado en la carpeta ResultImages!"<<endl;

}


void imgPreOrden(Entity<Layer_struct>* node, PixelGraph* pixel_generator, int x, int y){
    if (node->data->layer->getWidth() > x) x = node->data->layer->getWidth();
    if (node->data->layer->getHeight() > y) y = node->data->layer->getHeight();

    cout<<"\tcargando capa..."<<endl;
    pixel_generator->addToContext(node->data->layer->pixelArt(x,y));

    if (node->left) imgPreOrden(node->left,pixel_generator, x,y);
    if (node->right) imgPreOrden(node->right,pixel_generator, x,y);
}

void imgInOrden(Entity<Layer_struct>* node, PixelGraph* pixel_generator, int x, int y){
    if (node->data->layer->getWidth() > x) x = node->data->layer->getWidth();
    if (node->data->layer->getHeight() > y) y = node->data->layer->getHeight();

    if (node->left) imgPreOrden(node->left,pixel_generator,x,y);
    cout<<"\tcargando capa..."<<endl;
    pixel_generator->addToContext(node->data->layer->pixelArt(x,y));
    if (node->right) imgPreOrden(node->right,pixel_generator,x,y);
}

void imgPostOrden(Entity<Layer_struct>* node, PixelGraph* pixel_generator, int x, int y){
    if (node->data->layer->getWidth() > x) x = node->data->layer->getWidth();
    if (node->data->layer->getHeight() > y) y = node->data->layer->getHeight();

    if (node->left) imgPreOrden(node->left,pixel_generator,x,y);
    if (node->right) imgPreOrden(node->right,pixel_generator,x,y);

    cout<<"\tcargando capa..."<<endl;
    pixel_generator->addToContext(node->data->layer->pixelArt());
}

void generateBySearch(){
    Entity<Layer_struct>* root = bt_layers.getRoot();
    if (!root) return;
    string option;
    cout<<"-------------------------------------------------"<<endl;
    cout<<"\t1) Recorrido preorden"<<endl;
    cout<<"\t2) Recorrido inorden"<<endl;
    cout<<"\t3) Recorrido postorden"<<endl;
    cout<<"\tQue tipo de recorrido deseas probar para generar la imagen: ";
    cin>>option;


    int maxX = root->data->layer->getWidth(); int maxY = root->data->layer->getHeight();
    if (option == "1"){
        PixelGraph pixel_generator("preorden_pixelArt", "preorden_pixelArt");
        imgPreOrden(root, &pixel_generator, maxX, maxY);
        pixel_generator.generateNewFiles(true);
    } else if (option == "2"){
        PixelGraph pixel_generator("inorden_pixelArt", "inorden_pixelArt");
        imgInOrden(root, &pixel_generator, maxX, maxY);
        pixel_generator.generateNewFiles(true);
    } else if (option == "3"){
        PixelGraph pixel_generator("postorden_pixelArt", "postorden_pixelArt");
        imgPostOrden(root, &pixel_generator, maxX, maxY);
        pixel_generator.generateNewFiles(true);
    }


    cout<<"\t\n ! PixelArt ha sido creado en la carpeta ResultImages!"<<endl;

}

/*Adicionales*/
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


/*CARGA MASIVA POR AUTOMATAS*/
void load_layers(){
    try {
        LinkedList<BlockCap> bloques = loadCap("data/capas.cap");
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
        LinkedList<BlockIm> bloques = loadIm("data/imagenes.im");
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
        LinkedList<UserUsr> users = loadUsr("data/usuarios.usr");
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
