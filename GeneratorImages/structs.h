#ifdef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <string>

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

class Layer{
    private:
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

        if (!prev) rh->firts = target->right;
        else prev->right = target->right;

        prev = nullptr;
        temp = ch->first;

        while (temp && temp->row < r){
            prev = temp;
            temp = temp->down;
        }

        if (!prev) ch->firts = target->down;
        else prev->down = target->down;

        delete target;
    }

    void insert(int r, int c, std::string hex){
        if (hex.empty()){
            remove(r, c)
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
        Pixel* new_pixel = new Pixel(r, h, hex);

        // Fila (row)
        if (!rh->first || rh->first->column > c){
            new_pixel->right = rh->first;
            rh->first = new_node;
        } else{
            Pixel* temp = rh->first;
            while(temp->right && temp->right->column < r){
                temp = temp->right;
            }
            new_node->right = temp->right;
            temp->right = new_node;
        }
         // Columna (column)
        if (!ch->first || ch->first->row > r){
            new_pixel->down = ch->first;
            ch->first = new_node;
        } else{
            Pixel* temp = ch->first;
            while(temp->down && temp->down->row < r){
                temp = temp->down;
            }
            new_node->down = temp->down;
            temp->down = new_node;
        }
    }

    Pixel* search(int r, int c) {
        H_layer* currRow = rowHeaderRoot;
        while (currRow && currRow->index < r) currRow = currRow->next;
        if (currRow && currRow->index == r) {
            Pixel* temp = currRow->first;
            while (temp && temp->column < c) temp = temp->right;
            if (temp && temp->column == c) return temp;
        }
        return nullptr;
    }

    ~Layer(){
        // tengo que borrar recorriendo todos los nodos segun r,c
    }

};

#endif // STRUCTS_H
