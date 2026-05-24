#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <iostream>
#include "structs.h"
#include "dotGenerator.h"
using namespace std;
using namespace dotGenerator;
using namespace st;

namespace definitions{

/*----------------------------LAYERS----------------------------*/
class Layer{
    private:
    int idLayer;
    H_layer* rowH;
    int sizeRows = 0;
    H_layer* colH;
    int sizeCols = 0;

    /*GRAPHVIZ*/
    SubGraph graph;
    string tittle;

    // 0 = col
    // 1 = row

    string getType(int type){
        return (type == 0) ? "col_" : "row_";
    }

    // 0 = col
    // 1 = row
    void setSameRank(int type){
        graph.removeKeyWord("rank=same" + getType(type));
        if (type == 0) { // columnas
            if (!colH) return;
            H_layer* temp = colH;
            string hcol_names[sizeCols+1];
            hcol_names[0] = "root";
            int c = 1;
            while (temp){
                hcol_names[c] = temp->nameNode;
                temp = temp->next;
                c++;
            }
            graph.setInsameRank(hcol_names, c);
        }
    }

    void setNewsConHCols(){
        if (!colH) return;
        H_layer* temp = colH;
        while (temp->next){
            graph.simpleConnectNode(temp->nameNode, temp->next->nameNode, "style=dashed, tailport=e, headport=w");
            temp = temp->next;
        }
    }

     void setHeaderGraphviz(H_layer* header, int type)
    {
        header->nameNode = getType(type) +  to_string(header->index);
        graph.insertNode(header->nameNode);

        if (type == 0) //conexion header ROWS
        {
            H_layer* temp_row = rowH;
            if (temp_row != nullptr){
                string id_con = "id=\"rowHeader\"";
                graph.removeKeyWord(id_con);
                graph.simpleConnectNode("root" , temp_row->nameNode, "style=dashed, " + id_con);
                while (temp_row->next){
                    graph.simpleConnectNode(temp_row->nameNode, temp_row->next->nameNode, "style=dashed, " + id_con);
                    temp_row = temp_row->next;
                }
            }
        }
        else {//conexion header COLS y rank=same
            H_layer* temp_col = colH;
            if (temp_col != nullptr){
                string id_con = "id=\"colHeader\"";
                string result = "{rank=same; root; ";
                graph.removeKeyWord(id_con);
                graph.removeKeyWord("rank=same; root; col_");
                graph.simpleConnectNode("root" , temp_col->nameNode, "style=dashed, " + id_con);
                while (temp_col){
                    result += temp_col->nameNode + "; ";
                    if(temp_col->next)
                    graph.simpleConnectNode(temp_col->nameNode, temp_col->next->nameNode, "style=dashed, " + id_con);
                    temp_col = temp_col->next;
                }
                graph.insertInContext(result+ "}\n");
            }
        }//graph.simpleConnectNode(prev->nameNode, header->nameNode, "style=dashed, tailport=s, headport=n");

    }

    H_layer* getOrCreateHeader(H_layer* &root, int idx, int type) {
        if (!root || idx < root->index) {
            H_layer* newHeader = new H_layer(idx);
            newHeader->next = root;
            setHeaderGraphviz(newHeader, type);
            //conexion header
            //graph.simpleConnectNode("root", newHeader->nameNode, "style=dashed");
            root = newHeader;

            if (type == 0) sizeCols++;
            else sizeRows++;
            return root;
        }

        H_layer* aux = root;
        while(aux->next && aux->next->index <= idx) {
            aux = aux->next;
        }


        if(aux->index == idx) {
            setHeaderGraphviz(aux, type);
            return aux;
        }

        H_layer *newHeader = new H_layer(idx);
        newHeader->next = aux->next;
        aux->next = newHeader;

        if (type == 0) sizeCols++;
        else sizeRows++;
        setHeaderGraphviz(newHeader, type);

        return newHeader;
    }
    public:
    Layer(string tittle) : rowH(nullptr), colH(nullptr), tittle(tittle) {
        graph.changeName(tittle);
        graph.setColor("white");
        graph.insertNode("root");
    }
    SubGraph* getGraph(){
        return &graph;
    }

    void remove(int r, int c){
        H_layer* rh = rowH;
        while(rh && rh->index < r){
            rh = rh->next;
        }
        H_layer* ch = colH;
        while(ch && ch->index < c){
            ch = ch->next;
        }

        if (!rh || !ch) return;

        Pixel* target = nullptr;
        Pixel* prev = nullptr;
        Pixel* temp = rh->first;

        while (temp && temp->column < c){
            prev = temp;
            temp = temp->right;
        }
        if (!temp || temp->column != c) return;
        target = temp;

        if (!prev) rh->first = target->right;
        else prev->right = target->right;

        prev = nullptr;
        temp = ch->first;

        while (temp && temp->row < r){
            prev = temp;
            temp = temp->down;
        }

        if (!prev) ch->first = target->down;
        else prev->down = target->down;

        delete target;
    }

    void setRowRankGraphviz(H_layer* header){
        graph.removeKeyWord("rank=same; row_" + to_string(header->index));
        string result= "rank=same; row_" + to_string(header->index) + "; ";
        Pixel* temp = header->first;
        while(temp){
            result += temp->nameNode + "; ";
            temp = temp->right;
        }
        graph.insertInContext("{"+result+"}");
    }


    void createInGraphviz(H_layer* hCol, H_layer* hRow, Pixel* actual, string pixel_color){
        graph.insertNode(actual->nameNode, pixel_color);
        //Recorrer Row y rank same
        Pixel* temp_row = hRow->first;
        if (temp_row != nullptr){
            string id_con = "id=\"row_"+ to_string(hRow->index) + "\"";
            string result= "rank=same; row_" + to_string(hRow->index) + "; ";
            graph.removeKeyWord(id_con);
            graph.removeKeyWord("rank=same; row_" + to_string(hRow->index));
            graph.simpleConnectNode(hRow->nameNode, temp_row->nameNode, "constraint=false, color=\"#322263\", " + id_con);

            while (temp_row){
                result += temp_row->nameNode + "; ";
                if(temp_row->right)graph.simpleConnectNode(temp_row->nameNode, temp_row->right->nameNode, "color=\"#322263\", " + id_con);
                temp_row = temp_row->right;
            }
            graph.insertInContext("{"+result+"}");
        }

         Pixel* temp_col = hCol->first;
        if (temp_col != nullptr){
            string id_con = "id=\"col_"+ to_string(hCol->index) + "\"";
            graph.removeKeyWord(id_con);
            graph.simpleConnectNode(hCol->nameNode, temp_col->nameNode, "constraint=false, color=\"#322263\", " + id_con);

            while (temp_col){
                if(temp_col->down)graph.simpleConnectNode(temp_col->nameNode, temp_col->down->nameNode, "color=\"#322263\", " + id_con);
                temp_col = temp_col->down;
            }
        }

    }
    /// row, colum
    void insert(int r, int c, std::string hex){
        if (hex.empty()){
            remove(r, c);
            return;
        }

        H_layer* rh = getOrCreateHeader(rowH, r, 1);
        H_layer* ch = getOrCreateHeader(colH, c, 0);
        string nameNode = "p_" + to_string(r) + to_string(c);

        Pixel* existing = search(r, c);
        if (existing){
            existing->color = hex;
            return;
        }

        Pixel* new_pixel = new Pixel(r, c, hex);
        new_pixel->nameNode = nameNode;

        //Primer pixel (row)
        if (!rh->first || rh->first->column > c){
            new_pixel->right = rh->first;
            rh->first = new_pixel;
        } else{
            Pixel* temp = rh->first;
            while(temp->right && temp->right->column < c){
                temp = temp->right;
            }
            new_pixel->right = temp->right;
            temp->right = new_pixel;
        }
        //Primer pixel (col)
        if (!ch->first || ch->first->row > r){
            new_pixel->down = ch->first;
            ch->first = new_pixel;
        } else{
            Pixel* temp = ch->first;
            while(temp->down && temp->down->row < r){
                temp = temp->down;
            }
            new_pixel->down = temp->down;
            temp->down = new_pixel;
        }
        createInGraphviz(ch, rh, new_pixel, hex);
    }

    Pixel* search(int r, int c) {
        H_layer* currRow = rowH;
        while (currRow && currRow->index < r) currRow = currRow->next;
        if (currRow && currRow->index == r) {
            Pixel* temp = currRow->first;
            while (temp && temp->column < c) temp = temp->right;
            if (temp && temp->column == c) return temp;
        }
        return nullptr;
    }

    void clear_tree(){
        H_layer* currRow = rowH;
        if (currRow == nullptr) return;

        while(currRow != nullptr){
            Pixel* temp = currRow->first;
            while (temp != nullptr){
                Pixel* next = temp->right;
                delete temp;
                temp = next;
            }

            H_layer* next_row = currRow->next;
            delete currRow;
            currRow = next_row;
        }

        H_layer* currCol = colH;
        while(currCol != nullptr){
            H_layer* temp_col = currCol->next;
            delete currCol;
            currCol = temp_col;
        }

        rowH = nullptr;
        colH = nullptr;
    }

    string pixelArt(int sizeX = 0, int sizeY = 0){
        if (sizeX == 0) sizeX = sizeRows;
        if (sizeY == 0) sizeY = sizeCols;
        string result = "subgraph " + tittle + "{\n";

        H_layer* currRow = rowH;
        if (currRow == nullptr) return "";

        while(currRow != nullptr){
            Pixel* temp = currRow->first;
            while (temp != nullptr){
                string row_ = to_string(abs(temp->row - sizeX));
                string col_ = to_string(temp->column);
                result += tittle +"_"+ col_ + row_ + " [fillcolor=\""+ temp->color+"\", color=none, pos=\""+ col_ +","+ row_ +"!\"];\n";
                temp = temp->right;
            }
            currRow = currRow->next;
        }
        return result + "}";
    }

    void checkTree(){
        H_layer * col = colH;
        H_layer * row = rowH;

        if (col == nullptr && row == nullptr) return;

        while(col != nullptr){
            col = col->next;
        }

        while(row != nullptr){
            row = row->next;
        }
    }

    ~Layer(){
        clear_tree();
    }

};


typedef struct Layer_struct{
    int id = 0;
    Layer* layer;
    std::string name = "";
    Layer_struct(Layer* layer) : layer (layer) {}
} Layer_struct;



/*----------------------------BST----------------------------*/
template <typename T>
class BST{
    private:
    Entity<T>* root = nullptr;
    /*GRAPHVIZ*/
    SubGraph graph;
    string tittle;
    string color_nodes;
    string tittle_nodes;

    //Metodo exclusivo para graphviz
    void setConnectionsGraphviz(Entity<T>* parent) {
        if (!parent) return;
        std::string parent_node = tittle_nodes + std::to_string(parent->id);
        if (parent->left){

            std::string nodeLeft = tittle_nodes + std::to_string(parent->left->id);
            graph.simpleConnectNode(parent_node, nodeLeft);
            setConnectionsGraphviz(parent->left);
        }

        if (parent->right){
            std::string nodeRight = tittle_nodes + std::to_string(parent->right->id);
            graph.simpleConnectNode(parent_node, nodeRight);
            setConnectionsGraphviz(parent->right);
        }
    };

    Entity<T>* insert(Entity<T>* parent, int id, T* data){
        if (parent == nullptr){
            Entity<T>* new_entity = new Entity<T>(id, data);
            new_entity->data->id = id;
            std::string node_string = tittle_nodes + std::to_string(id);
            new_entity->data->name = node_string;

            graph.insertNode(node_string);
            return new_entity;
        }

        if (id < parent->id)
            parent->left = insert(parent->left, id, data);

        if (id > parent->id)
            parent->right = insert(parent->right, id, data);
        return parent;
    }

    Entity<T>* search(Entity<T>* parent, int id_entity) {
        if (parent == nullptr) return nullptr;
        if (parent->id == id_entity) return parent;

        if (id_entity < parent->id)
            return search(parent->left, id_entity);
        else
            return search(parent->right, id_entity);
    }

    void updateEntity(int id_node, Entity<T>* new_entity){
        Entity<T> found = search();

        if (!found) return;
        found->data = new_entity->data;
    }

     Entity<T>* minValueNode(Entity<T>* node) {
        Entity<T>* current = node;

        while(current && current->left != nullptr) {
            current = current->left;
        }

        return current;
    }

    Entity<T>* remove(Entity<T>* node, int value) {

        if (node == nullptr)
            return node;

        if (value < node->value) {
            node->left = remove(node->left, value);
        } else if (value > node->value) {
            node->right = remove(node->right, value);
        } else {

            // Caso 1: Hojas (nodos sin hijos)
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            else if (node->left == nullptr) {
                Entity<T>* temp = node->right;
                delete node;
                return temp;
            }

            else if (node->right == nullptr) {
                Entity<T>* temp = node->left;
                delete node;
                return temp;
            }


            Entity<T>* temp = minValueNode(node->right);
            node->value = temp->value;
            node->right = remove(node->right, temp->value);
        }
    }

    void clear_postorden(Entity<T>* parent){
        if (parent == nullptr) return;

        clear_postorden(parent->left);
        clear_postorden(parent->right);
        delete parent;
    }

    void f_preOrden(Entity<T>* parent){
        if (parent == nullptr) return;

        if (parent->left || parent->right) std::cout<<std::endl<<parent->id<<std::endl;
        if (parent->left) std::cout<<" Izq: "<<parent->left->id<<" ";
        if (parent->right) std::cout<<" Der: "<<parent->right->id<<" ";
        f_preOrden(parent->left);
        f_preOrden(parent->right);
    }

    int height(Entity<T>* parent){
        if (!parent) return 0;

        int h_left = height(parent->left);
        int h_right = height(parent->right);

        return std::max(h_left, h_right) + 1;
    }

    /*void bsf(){
        Queue<Entity<T>> queue_;
        queue_.enqueue(root, false);
        int position_arr = 0;
        while (!queue_.isEmpty()){
            Node<Entity<T>>* temp = queue_.dequeue();
            std::cout<<" "<<temp->data->id<<" ";
            std::cout<<"Height: "<<height(temp->data);

            if (temp->data->left) queue_.enqueue(temp->data->left, false);
            if (temp->data->right) queue_.enqueue(temp->data->right, false);

            delete temp;
        }
    }*/

    public:

    BST(string tittle, string tittle_nodes, string color_nodes) : tittle(tittle), tittle_nodes(tittle_nodes), color_nodes(color_nodes) {
        graph.changeName(tittle);
        graph.setColor(color_nodes);
    }

    SubGraph * getGraph() { return &graph;}

    void insert(int id, T* data){
        root = insert(root, id, data);
        preOrden_graphviz();
    }

    Entity<T>* search(int id){
        Entity<T>* found = search(root, id);
        if (!found) return nullptr;
        return found;
    }

    void print_preorden(){
        if (root == nullptr){
            std::cout<<"Nada que mostrar en el BST"<<std::endl;
            return;
        }
        std::cout<<"En preornde:"<<std::endl;
        f_preOrden(root);
    }

    void print_bsf(){
        if (root == nullptr){
            std::cout<<"Nada que mostrar en el BST"<<std::endl;
            return;
        }
        std::cout<<"En BSF:"<<std::endl;
        //bsf();
    }

    void preOrden_graphviz(){
        graph.clearAllConections("dashed");
        setConnectionsGraphviz(root);
    }

    ~BST(){
        clear_postorden(root);
    }
};


typedef struct Image{
    int id = 0;
    string name = "";
    LinkedList<Layer_struct> layers;
    Image(std::string name) :name(name) {}
} Image;

typedef BiNode<Image> ImageBiNode;
typedef Node<Image> ImageNode;


//Imagenes globales
class ListImages : private DoubleLinkedList<Image>{
private:
    int globalID = 0;
    int size_doubleLinkedList;
    SubGraph graph;
    string tittle;
    string tittle_nodes;
    string color_nodes;

    string tittle_subNodes;

    string* dll_to_array(){
        if (size_doubleLinkedList == 0) return nullptr;
        string* arr = new string[size_doubleLinkedList];
        ImageBiNode* temp = head;
        int c = 0;
        while(temp != nullptr){
            arr[c] = tittle_nodes +to_string(temp->data->id);
            temp = temp->next;
            c++;
        }
        return arr;
    }

    void setRankGraphviz() {
        string* arr = dll_to_array();
        graph.updateSameRank(arr, size_doubleLinkedList);
        delete[] arr;
    }

    ImageBiNode* createNode(int id,Image* data){
        ImageBiNode * new_node = new ImageBiNode(data ,true);
        new_node->id = id;
        new_node->data->id = id;
        new_node->next = nullptr;
        new_node->prev = nullptr;

        string nameNode = tittle_nodes +to_string(new_node->data->id);
        new_node->nameNode = nameNode;
        size_doubleLinkedList++;
        graph.insertNode(nameNode);
        return new_node;
    }


    void afterInsertAction(ImageBiNode* temp) override{
        if(temp->next != nullptr){
            string nodeA = temp->nameNode;
            string nodeB = temp->next->nameNode;
            graph.simpleConnectNode(nodeA, nodeB);
            graph.simpleConnectNode(nodeB, nodeA);
        }


    }
public:
    SubGraph * getGraph() { return &graph;}

    ListImages (string tittle, string tittle_nodes, string color_nodes, string tittle_subNodes) : tittle(tittle), tittle_nodes(tittle_nodes), color_nodes(color_nodes), tittle_subNodes(tittle_subNodes) {
        graph.changeName(tittle);
        graph.setColor(color_nodes);
    }

    void add(int id, Image* data){
        ImageBiNode* new_node = createNode(id, data);
        insert(new_node);
        setRankGraphviz();
    }

    void addLayer(int idImage, Layer_struct* layer){
        if (!layer) return;
        ImageBiNode* node =  getByIndex(idImage);
        if(!node) return;

        Node<Layer_struct>* temp_node = new Node(layer, false);
        string subNode_name = "i"+to_string(node->id)+"_"+tittle_subNodes +to_string(temp_node->data->id);
        temp_node->nameNode = subNode_name;
              if (node->data->layers.isEmpty()) {
            //conexion inicial
            graph.simpleConnectNode(node->nameNode, subNode_name);
        } else{
            //concatenar LL
            graph.simpleConnectNode(node->data->layers.getLast()->nameNode, subNode_name);
        }
        node->data->layers.insert_node(temp_node);

        graph.insertNode(subNode_name);
        graph.simpleConnectNode(subNode_name, temp_node->data->name);
    }

};

//Utilizado unicamente para tener la referencia de las imagenes por cada usuario y ahorrar su busqueda en toda la DLL
class SimpleListImages: private LinkedList<Image>{
private:
    int size_linkedList;
    int globalID = 0;
    SubGraph* graph;
    string tittle_nodes;
    string color_nodes;
    string tittle_subNodes;

    ImageNode* createNode(int id, Image* data){
        ImageNode * new_node = new ImageNode(data ,true);
        new_node->id = id;
        new_node->data->id = id;
        new_node->next = nullptr;
        string nameNode = tittle_nodes + "_img" +to_string(new_node->data->id);
        new_node->nameNode = nameNode;
        size_linkedList++;
        string configNode = "shape=record, style=filled, fillcolor="+color_nodes;
        graph->insertNode_extend(nameNode, configNode);
        if (isEmpty())
            graph->simpleConnectNode(tittle_nodes, nameNode, "style=dashed");
        else {
            afterInsertAction(getLast(), new_node);
        }

        graph->simpleConnectNode(nameNode, "img_" + to_string(data->id), "style=dashed");
        return new_node;
    }

    //Conexion de nodos en graphviz LinkedList
    void afterInsertAction(ImageNode* temp, ImageNode* actual) {

        graph->simpleConnectNode(temp->nameNode, actual->nameNode, "style=dashed");

    }

public:
    void setSubgraph(SubGraph* subgraph){
        graph = subgraph;
    }

    SimpleListImages(string tittle_nodes, string color_nodes) : tittle_nodes(tittle_nodes), color_nodes(color_nodes){

    }

    void add(int id,Image* data, SubGraph* subgraph){
        if (graph != subgraph) setSubgraph(subgraph);
        ImageNode* new_node = createNode(id, data);
        this->insert(new_node);
        //setRankGraphviz();
    }
};

typedef struct User{
    int id = 0;
    string name;
    string name_subNodes = "";
    string color_subNodes = "";

    SimpleListImages list_images;
    User(string name_subNodes, string color_subNodes = "\"#7F008F\"") : name_subNodes(name_subNodes), color_subNodes(color_subNodes),
    list_images(name_subNodes, color_subNodes)
    {}

} User;



}
#endif
