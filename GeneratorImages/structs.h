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
    int id = 0;
    struct Entity* left = nullptr;
    struct Entity* right = nullptr;

    Entity(int id_, T* data) : data(data), left(nullptr), right(nullptr), id(id_) { }

    ~Entity(){
        delete data;
    }
};

typedef struct User{
    std::string name = "";
    User(std::string name) : name(name) {}
} User;


template <typename T>
    struct Node{
        int id = 0;
        T* data;
        std::string nameNode = "";
        struct Node* next;

        Node(T* data, bool owns = true) : data(data), owns_data(owns), next(nullptr) {}
        Node() : data(new T()), owns_data(true), next(nullptr) {}

        ~Node() {
            if (owns_data) delete data;
        }

        private:
            bool owns_data;
    };

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

template<typename T>
class LinkedList{
    protected:
        struct Node<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

        virtual void afterInsertAction(struct Node<T>* temp) {};

        int insert(struct Node<T>* new_node){
            if (checkList() == 1){
                head = new_node;
                head->next = nullptr;
                return 0;
            }
            else{
                struct Node<T>* temp = head;
                while(temp->next != nullptr){
                    temp = temp->next;
                }

                temp->next = new_node;
                afterInsertAction(temp);
                return 0;
            }
            return 1;
        }

        struct Node<T>* getByIndex(int index){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while(temp != nullptr){
                    if (temp->id == index){
                        return temp;
                    }
                    temp = temp->next;
                }
            }
            return nullptr;
        }

        struct Node<T>* getFirst(){
            if (checkList() ==0 ){
                return head;
            }
            return nullptr;
        };

        virtual void printData(struct Node<T>* node) {};

        void showList(){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while(temp != nullptr){
                    printData(temp);
                    temp = temp->next;
                }
            }
        }

    void clearList(){
        struct Node<T>* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }


    public:
        ~LinkedList(){
            clearList();
        }


        T** to_array(){
            int size = 0;
            Node<T>* temp = head;

            while (temp != nullptr){
                size++;
                temp = temp->next;
            }

            if (size == 0) return nullptr;

            T** arr = new T*[size];

            temp = head;

            for (int i = 0; i < size; i++){
                arr[i] = temp->data;
                temp = temp->next;
            }

            return arr;
        }


    };

    template <typename T>
    class Queue : protected LinkedList<T>{
        using LinkedList<T>::head;

        public:
            struct Node<T>* back(){
                struct Node<T>* temp = head;
                while(temp->next != nullptr){
                    temp = temp->next;
                }
                return temp;
            }

            struct Node<T>* peek(){
                return head;
            }

            struct Node<T>* dequeue(){
                if (head == nullptr) return nullptr;

                struct Node<T>* temp = head;
                head = head->next;
                temp->next = nullptr;
                return temp;
            }

            bool isEmpty(){
                if (head == nullptr) return true;
                else return false;
            }

            void enqueue(T* data, bool owns = true){
                Node<T>* node = new Node<T>(data, owns);
                this->insert(node);
            }

            T* to_array(){
                return this->to_array();
            }
    };


template <typename T>
class BST{
    private:
    Entity<T>* root = nullptr;

    Entity<T>* insert(Entity<T>* parent, int id, T* data){
        if (parent == nullptr){
            return new Entity<T>(id, data);
        }

        if (id < parent->id) parent->left = insert(parent->left, id, data);
        if (id > parent->id) parent->right = insert(parent->right, id, data);

        return parent;
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

    void BSF(){
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
    }

    public:
    void insert(int id, T* data){
        root = insert(root, id, data);
    }

    void print_bts(){
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
        BSF();
    }

    ~BST(){
        clear_postorden(root);
    }
};

}


#endif // STRUCTURES_H
