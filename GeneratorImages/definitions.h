#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <iostream>
#include "structs.h"
#include "dotGenerator.h"
using namespace std;
using namespace dotGenerator;
using namespace st;

namespace definitions{

/*LAYERS*/
class Layer{
    private:
    int idLayer;
    H_layer* rowH;
    H_layer* colH;

    H_layer* getOrCreateHeader(H_layer* &root, int idx) {
        if (!root || idx < root->index) {
            H_layer* newHeader = new H_layer(idx);
            newHeader->next = root;
            root = newHeader;
            return root;
        }

        H_layer* aux = root;
        while(aux->next && aux->next->index <= idx) {
            aux = aux->next;
        }

        if(aux->index == idx) return aux;

        H_layer *newHeader = new H_layer(idx);
        newHeader->next = aux->next;
        aux->next = newHeader;
        return newHeader;
    }
    public:
    Layer() : rowH(nullptr), colH(nullptr) {}

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

    void insert(int r, int c, std::string hex){
        if (hex.empty()){
            remove(r, c);
            return;
        }

        H_layer* rh = getOrCreateHeader(rowH, r);
        H_layer* ch = getOrCreateHeader(colH, c);

        Pixel* existing = search(r, c);
        if (existing){
            existing->color = hex;
            return;
        }

        Pixel* new_pixel = new Pixel(r, c, hex);

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
                std::cout<<temp->color<<std::endl;
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

    void checkTree(){
        H_layer * col = colH;
        H_layer * row = rowH;

        if (col == nullptr && row == nullptr) return;

        while(col != nullptr){
            std::cout<<col->index<<std::endl;
            col = col->next;
        }

        while(row != nullptr){
            std::cout<<row->index<<std::endl;
            row = row->next;
        }
    }

    ~Layer(){
        clear_tree();
    }

};

/*----------------------------BST----------------------------*/
template <typename T>
class BST{
    private:
    Entity<T>* root = nullptr;
    /*GRAPHVIZ*/
    string tittle;
    string color_nodes;
    string tittle_nodes;
    SubGraph graph;

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

        //if (!parent || (!parent->left && !parent->right)) return;
        /*std::string parent_node = tittle_nodes + std::to_string(parent->id);
        if (parent->left)
        {
            std::string nodeLeft = tittle_nodes + std::to_string(parent->left->id);
            graph.simpleConnectNode(parent_node, nodeLeft);
        }
        if (parent->right)
        {
          std::string nodeRight = tittle_nodes + std::to_string(parent->right->id);
            graph.simpleConnectNode(parent_node, nodeRight);
        }*/
    };

    Entity<T>* insert(Entity<T>* parent, int id, T* data){
        if (parent == nullptr){
            Entity<T>* new_entity = new Entity<T>(id, data);
            new_entity->data->id = id;
            std::string node_string = tittle_nodes + std::to_string(id);
            graph.insertNode(node_string);
            return new_entity;
        }

        if (id < parent->id) parent->left = insert(parent->left, id, data);
        if (id > parent->id) parent->right = insert(parent->right, id, data);
        return parent;
    }

    Entity<T>* search(Entity<T>* parent,int id_entity){
        if (!parent) return nullptr;
        if (parent->id == id_entity){
            return parent;
        }

        if (id_entity < parent->id) search(parent->left, id_entity);
        if (id_entity > parent->id) search(parent->right, id_entity);
    }

    void updateEntity(int id_node, Entity<T>* new_entity){
        Entity<T> found = search();

        if (!found) return;
        found->data = new_entity->data;
    }

    /*void delete(){
        //luego
    }*/

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
    }

    SubGraph * getGraph() { return &graph;}

    void insert(int id, T* data){
        root = insert(root, id, data);
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
        setConnectionsGraphviz(root);
    }

    ~BST(){
        clear_postorden(root);
    }
};


}
#endif
