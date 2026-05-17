#ifndef DOT_GENERATOR
#define DOT_GENERATOR

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "structs.h"


namespace dotGenerator{
    using namespace std;
    using namespace st;

    class SubGraph{
        private:
        string tittle;
        string context; //aca voy a colocar todo en un texto en conjunto, luego lo meto todo al dotfile

        public:
            string getTittle() const {return tittle;}
            string getContext() const { return context;}

            void changeName(string new_name){
                if (context == ""){
                    context = "subgraph cluster_" + new_name +"{\nlabel=\""+ new_name +"\"\nstyle = \"filled,rounded\";\ncolor = lightgrey;\n";
                } else{
                    int pos = context.find("{");
                    string context_wth_name = context.substr(pos+1);
                    context = "subgraph cluster_" + new_name +"{\nlabel=\""+ new_name +"\"\nstyle = \"filled,rounded\";\ncolor = lightgrey;\n" + context_wth_name;
                }
                tittle = new_name;

            }

            void insertNode(string name){
                if (context == ""){
                    changeName("na");
                }
                context += name + "[label=\"" + name +"\", shape=record, style=filled, fillcolor=white];\n";
            }

            void insertNode(string name, string label){
                if (context == ""){
                    changeName("na");
                }
                context +=  name + "[label=\"" + label +"\", shape=record, style=filled, fillcolor=white];\n";
            }

            void simpleConnectNode(string nodeA, string nodeB){
                context += nodeA + " -> " + nodeB +";\n";
            }


            void removeNode(string node_delete, string node_prev = "", string node_next = ""){
                stringstream separator(context); //Clase std::stringstream que me va a servir para separar el context en lineas sin hacer un bucle anidado y evitar un O(n2)
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    size_t position = temp.find(node_delete);
                    if (position != string::npos) {
                        char next = temp[position + node_delete.size()];
                        if (next == '[' || next == ' ' || next == ';'){ continue;}
                    }
                    new_context+=temp+"\n";
                }
                context = new_context;
                if (node_prev != "" && node_next != "") simpleConnectNode(node_prev, node_next);
            }

            void updateLabelNode(string nameNode, string label){
                stringstream separator(context);
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    size_t pos = temp.find(nameNode+'[');
                    if (pos != string::npos){
                        new_context+= nameNode + "[label=\"" + label +"\", shape=record, style=filled, fillcolor=white];\n";
                        continue;
                    }
                    new_context+=temp+"\n";
                }
                context = new_context;

            }


            void removeAllConections(){
                stringstream separator(context);
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    if (temp.find("->") != string::npos) {continue;}
                    else {new_context+=temp+"\n";}
                }
                context = new_context;
            }


            void removeBiNode(){
                //removerNodos
                //unir nodo anterior con el nodo siguietnte
                //unir nodo siguiente con el nodo anterior
            }

    };


    //LISTA DE SUBGRAPHS (ESTRUCTURA DE APOYO)
    typedef Node<SubGraph> SubGraphNode;

    class SubgraphQueue : private LinkedList<SubGraph>{
        using LinkedList::head;

        private:
        SubGraphNode* createNode(SubGraph* subgraph){
            SubGraphNode* new_node = new SubGraphNode;
            new_node->data = subgraph;
            new_node->next = nullptr;
            return new_node;
        }

        void printData(SubGraphNode* node) override{ //DEBUG METHOD
           cout<<" - Nodo: "<< node->data->getTittle()<<endl;
        }

        public:
        void add(SubGraph* subgraph){
            SubGraphNode* new_node = createNode(subgraph);
            insert(new_node);
        }

        string getContexts(){
            string data = "";
            if (head != nullptr){
                SubGraphNode* temp = head;
                while(temp != nullptr){
                    string temp_context = temp->data->getContext();
                    data += temp_context + "\n}\n\n";
                    temp = temp->next;
                }
            }
            return data;
        }

        void print(){
            showList();
        }
    };

    class DotFile{
        private:
            string tittle;
            string context;
            SubgraphQueue queue_subgraph;

            void constructFile(const string filename) {
                ofstream outFile(filename);

                if (!outFile) return;

                outFile << context;
                outFile.close();
            }

            int generateFile(){
                constructFile("graph"+tittle+".dot");

                string result_str = "dot - Tpng graph" + tittle + ".dot -o output.png";
                int result = system(result_str.c_str());
                return result;
            }

        public:

            SubgraphQueue* getQueue() {return &queue_subgraph;}
            void resetContext() {
                context = "digraph "+tittle+" {}";
                generateFile();
            }

            int updateSubGraphs(){
                // rankdir=LR;
                if (context != "digraph "+tittle+" {\n\n" + queue_subgraph.getContexts() +"\n}")
                {
                     context = "digraph "+tittle+" {\n\n" + queue_subgraph.getContexts() +"\n}";
                     return 0;
                }
                return -1;

            }

            int generateNewFiles(){
                //queue_subgraph.print();

                return (updateSubGraphs() == 0) ? generateFile() : 2;

            }

    };


}

#endif // DOT_GENERATOR
