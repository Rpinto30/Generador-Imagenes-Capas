#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <string>

namespace st{
    typedef struct Pixel{
    int row;
    int column;
    std::string color;
    std::string nameNode;
    Pixel* right;
    Pixel* down;

    Pixel (int r, int c, std::string hex) : row(r), column(c), color(hex), right(nullptr), down(nullptr) {}
} Pixel;

typedef struct H_layer{
    int index;
    H_layer* next;
    Pixel* first;
    std::string nameNode;

    H_layer(int i) : index(i), next(nullptr), first(nullptr) {}
} H_layer;


// PLANTILLA GENERALIZADA PARA LOS NODOS
template <typename T>
struct Entity{
    T* data;
    struct Entity* left = nullptr;
    struct Entity* right = nullptr;
    int id;
    int height;

    Entity(int id_, T* data) : data(data), left(nullptr), right(nullptr), id(id_) { }

    ~Entity(){
        delete data;
    }
};



template <typename T>
struct Node{
        int id = 0;
        T* data;
        std::string nameNode = "";
        struct Node* next;

        Node(T value, bool owns = true)
            : owns_data(owns), id(0), data(new T(value)), nameNode(""), next(nullptr) {
        }
        Node(T* data, bool owns = true) : data(data), owns_data(owns), next(nullptr) {}
        Node() : data(new T()), owns_data(true), next(nullptr) {}

       ~Node() {
            if (!owns_data && data != nullptr) {
                delete data;
                data = nullptr;
            }
        }

        private:
            bool owns_data;
};

template <typename T>
struct BiNode{
        int id;
        T* data;
        std::string nameNode = "";
        struct BiNode* next;
        struct BiNode* prev;

         BiNode(T* data, bool owns = true) : data(data), owns_data(owns), next(nullptr), prev(nullptr) {}
         //BiNode() : data(new T()), next(nullptr), prev(nullptr) {} //generate their own data
       /* ~BiNode() {
            if(owns_data) delete data;
        }*/

         private:
            bool owns_data;
    };


/*----------------------------LINKEDLIST----------------------------*/
template<typename T>
class LinkedList{
    protected:
        struct Node<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

        virtual void afterInsertAction(Node<T>* temp) {};

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

    void deleteNode(int id){

        while (head && head->id == id) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }


        Node<T>* temp = head;
        while (temp && temp->next) {
            if (temp->next->id == id) {
                Node<T>* temp_2 = temp->next;
                temp->next = temp->next->next;
                delete temp_2;
                break;
            } else {
                temp = temp->next;
            }
        }

    }


    public:
        ~LinkedList(){
            clearList();
        }

        void insert_node(struct Node<T>* node){
            insert(node);
        }

        void insert_node(T data){
            Node<T>* new_node = new Node<T>(data,false);
            insert(new_node);
        }

        int size(){
            Node<T>* temp = head;
            int sz = 0;
            while (temp){
                sz++;
                temp = temp->next;
            }
            return sz;
        }

        bool isEmpty(){
            if (head) return false;
            else return true;
        }


        Node<T>* getHead(){
            if (head) return head;
            return nullptr;
        }

        Node<T>* getLast(){
            if (head == nullptr) return nullptr;

            Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            return temp;
        }

        T* getIndex(int index){
            if (checkList() == 0){
                Node<T>* temp = head;
                while(temp != nullptr){
                    if (temp->id == index){
                        return temp->data;
                    }
                    temp = temp->next;
                }
            }
            return nullptr;
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

       struct Iterator {
    Node<T>* current;

    Iterator(Node<T>* node) : current(node) {}

    T& operator*() { return *(current->data); }

    Iterator& operator++() {
        current = current->next;
        return *this;
    }

    bool operator!=(const Iterator& other) const {
        return current != other.current;
    }

    bool operator==(const Iterator& other) const {
        return current == other.current;
    }
};

    Iterator begin() const { return Iterator(head); }
    Iterator end() const   { return Iterator(nullptr); }


    };


/*----------------------------QUEUE----------------------------*/
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


/*----------------------------DOUBLELINKEDLIST----------------------------*/
    template <typename T>
    class DoubleLinkedList{
    protected:
        struct BiNode<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

        virtual void afterInsertAction(struct BiNode<T>* temp) {};

        int insert(struct BiNode<T>* new_node){
            if (checkList() == 1){
                head = new_node;
                head->next = nullptr;
                head->prev = nullptr;
                return 0;
            }
            else{
                struct BiNode<T>* temp = head;
                while(temp->next != nullptr){
                    temp = temp->next;
                }
                temp->next = new_node;
                new_node->prev = temp;
                new_node->next = nullptr;
                afterInsertAction(temp);
                return 0;
            }
            return 1;
        }

        struct BiNode<T>* getByIndex(int index){
            if (checkList() == 0){
                struct BiNode<T>* temp = head;
                while(temp != nullptr){
                    if (temp->id == index){
                        return temp;
                    }
                    temp = temp->next;
                }
            }
            return nullptr;
        }

        struct BiNode<T>* getFirst(){
            if (checkList() ==0 ){
                return head;
            }
            return nullptr;
        };

        T* getIndex(int index){
            if (checkList() == 0){
                BiNode<T>* temp = head;
                while(temp != nullptr){
                    if (temp->id == index){
                        return temp->data;
                    }
                    temp = temp->next;
                }
            }
            return nullptr;
        }

        virtual void printData(struct BiNode<T>* node) {};

        void showList(){
            if (checkList() == 0){
                struct Node<T>* temp = head;
                while (temp != nullptr) {
                    printData(temp);
                    temp = temp->next;
                }
            }
        }

        ~DoubleLinkedList(){
            struct BiNode<T>* temp;
            while (head) {
                temp = head;
                head = head->next;
                delete temp;
            }

        }
    };

    template <typename T>
    class DoubleCircleList{
    protected:
        struct BiNode<T>* head = nullptr;

        int checkList(){
            if (head == nullptr) return 1;
            return 0;
        }

        virtual void afterInsertAction(struct BiNode<T>* temp) {}

        int insert(struct BiNode<T>* new_node){
            if (checkList() == 1){
                head = new_node;
                head->next = new_node;
                head->prev = new_node;
            }
            else{
                struct BiNode<T> * last = head->prev;

                last->next = new_node;
                head->prev = new_node;

                new_node->next = head;
                new_node->prev = last;
            }
            afterInsertAction(new_node);
            return 0;
        }

        struct BiNode<T>* getActual(){
            return head;
        }

        void modeForward(){
            head = head->next;
        }

        //hacer la conversion de Node a BiNode
        struct BiNode<T>* removeActual(){
                if (checkList() == 1) return nullptr;

                struct BiNode<T>* temp = head;

                if (head->next == head) {
                    head = nullptr;
                } else {
                    struct BiNode<T>* last = head->prev;
                    head = temp->next;
                    last->next = head;
                    head->prev = last;
                }

                temp->next = nullptr;
                temp->prev = nullptr;
                return temp;
        }

        BiNode<T>* getByIndex(int id){
            BiNode<T>* temp = getActual();
            do{
                if(temp->id == id) return temp;
                temp = temp->next;
            } while(temp != getActual());
            return nullptr;
        }

        T* getIndex(int id){
            BiNode<T>* temp = getActual();
            do{
                if(temp->id == id) return temp->data;
                temp = temp->next;
            } while(temp != getActual());
            return nullptr;
        }


        void desligateNode(struct BiNode<T> * node){
            if (node->next == node) {
                head = nullptr;
            } else {
                BiNode<T>* front_ = node->next;
                BiNode<T>* back_  = node->prev;
                back_->next = front_;
                front_->prev = back_;
                if (node == head) head = front_;
            }
            node->next = nullptr;
            node->prev = nullptr;

        }

        ~DoubleCircleList(){
            if (head == nullptr) return;
                head->prev->next = nullptr;
                BiNode<T>* temp = head;
                while (temp != nullptr) {
                    BiNode<T>* deleter = temp;
                    temp = temp->next;
                    delete deleter;
            }
        }
    };


}


#endif // STRUCTURES_H
