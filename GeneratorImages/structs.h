#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <string>

namespace st{
    typedef struct Pixel{
    int row;
    int column;
    std::string color;
    Pixel* right;
    Pixel* down;

    Pixel (int r, int c, std::string hex) : row(r), column(c), color(hex), right(nullptr), down(nullptr) {}
} Pixel;

typedef struct H_layer{
    int index;
    H_layer* next;
    Pixel* first;

    H_layer(int i) : index(i), next(nullptr), first(nullptr) {}
} H_layer;


// PLANTILLA GENERALIZADA PARA LOS NODOS
template <typename T>
struct Entity{
    T* data;
    struct Entity* left;
    struct Entity* right;

    Entity() : data(new T), left(nullptr), right(nullptr) {}
};


class Layer{
    private:
    int idLayer;
    H_layer* rowH;
    H_layer* colH;

    H_layer* getOrCreateHeader(H_layer* &root, int idx) {
        if (!root || idx < root->index) { //Sin punto de inicio o index de inserción es el menor (Headers)
            H_layer* newHeader = new H_layer(idx);
            newHeader->next = root;
            root = newHeader;
            return root;
        }

        // LinkedList moment
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
        // ENCABEZADOS
        H_layer* rh = rowH;
        while(rh && rh->index < r){
            rh = rh->next;
        }
        H_layer* ch = colH;
        while(ch && ch->index < c){
            ch = ch->next;
        }

        if (!rh || !ch) return;

        // ENCONTRAR REFERENCIAS TANTO EN rh como en ch
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

        // Buscar existencias
        Pixel* existing = search(r, c);
        if (existing){
            existing->color = hex;
            return;
        }

        // Creacion nuevos pixeles
        Pixel* new_pixel = new Pixel(r, c, hex);

        // Fila (row)
        if (!rh->first || rh->first->column > c){
            new_pixel->right = rh->first;
            rh->first = new_pixel;
        } else{
            Pixel* temp = rh->first;
            while(temp->right && temp->right->column < r){
                temp = temp->right;
            }
            new_pixel->right = temp->right;
            temp->right = new_pixel;
        }
         // Columna (column)
        if (!ch->first || ch->first->row > r){
            new_pixel->down = ch->first;
            ch->first = new_pixel;
        } else{
            Pixel* temp = ch->first;
            while(temp->down && temp->down->row < c){
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
        if (currCol == nullptr) return;
        while(currCol != nullptr){
            H_layer* temp_col = currCol->next;
            delete temp_col;
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
        // tengo que borrar recorriendo todos los nodos segun r,c
        clear_tree();
    }

};


class BTS{

};


class ListImages{

};

class QueueLayers{

};



}


#endif // STRUCTURES_H
