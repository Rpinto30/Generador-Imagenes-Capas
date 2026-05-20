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
    int id;
    struct Entity* left;
    struct Entity* right;

    Entity(int id_, T* data) : data(data), left(nullptr), right(nullptr), id(id_) { }

    ~Entity(){
        delete data;
    }
};

typedef struct User{
    int id = 0;
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

       /* ~Node() {
            if (owns_data && data) delete data;
        }*/

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

        struct BiNode<T>* getFirst(){
            if (checkList() ==0 ){
                return head;
            }
            return nullptr;
        };

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

        //Lo del metodo desctructor lo investigue de: https://medium.com/@RobuRishabh/beginners-guide-to-linked-list-in-c-d8445ef906ab
        ~DoubleLinkedList(){
            struct BiNode<T>* temp;
            while (head) {
                temp = head;
                head = head->next;
                delete temp;
            }

        }
    };



}


#endif // STRUCTURES_H
