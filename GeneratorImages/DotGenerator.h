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
        string context;
        string color = "white";

        public:
            string getTittle() const {return tittle;}
            string getContext() const { return context;}
            void setColor(string col) {
                this->color = col;
            }

            void changeName(string new_name){
                if (context == ""){
                    context = "subgraph cluster_" + new_name +"{\nlabel=\""+ new_name +"\"\nstyle = \"filled,rounded\"; ordering=out;\n";
                } else{
                    int pos = context.find("{");
                    string context_wth_name = context.substr(pos+1);
                    context = "subgraph cluster_" + new_name +"{\nlabel=\""+ new_name +"\"\nstyle = \"filled,rounded\";\ncolor = "+color+";\n" + context_wth_name;
                }
                tittle = new_name;

            }

            void insertInContext(string st_){
                context+=st_;
            }

            void insertNode(string name){
                if (context == ""){
                    changeName("na");
                }
                context += name + "[label=\"" + name +"\", shape=box, style=filled, fillcolor="+color+"];\n";
            }

            void insertNode(string name, string label){
                if (context == ""){
                    changeName("na");
                }
                context += name + "[label=\"" + label +"\", shape=box, style=filled, fillcolor="+color+"];\n";
            }

              void insertNode_extend(string name, string config){
                if (context == ""){
                    changeName("na");
                }
                context +=  name + "[label=\"" + name + "\"" + config + "];\n";
            }


            void clearAllConections(){
                stringstream separator(context);
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    size_t position = temp.find("->");
                    if (position != string::npos)  continue;
                    new_context+=temp+"\n";
                }
                context = new_context;
            }

             void removeKeyWord(string keyWord){
                stringstream separator(context);
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    size_t position = temp.find(keyWord);
                    if (position != string::npos)  continue;
                    new_context+=temp+"\n";
                }
                context = new_context;
            }

              void clearAllConections(string condition){
                stringstream separator(context);
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    size_t position = temp.find("->");
                    size_t position_condition = temp.find(condition);
                    if (position != string::npos && position_condition == string::npos) continue;
                    new_context+=temp+"\n";
                }
                context = new_context;
            }

            void setInsameRank(string* nodes, int size){
                if (context == ""){
                    changeName("na");
                }

                string rank_ = "\n{rank=same; ";
                for (int i = 0; i < size; i++) {
                    rank_ += " " + *(nodes+i) + ";";
                }

                context += rank_ + " }\n";

            }

            void updateSameRank(string* nodes, int size){
                stringstream separator(context);
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    if (temp.find("{rank=same;") != string::npos) {continue;}
                    else {new_context+=temp+"\n";}
                }
                context = new_context;

                setInsameRank(nodes, size);
            }

            void simpleConnectNode(string nodeA, string nodeB){
                context += nodeA + " -> " + nodeB +" [color="+color+"];\n";
            }

             void simpleConnectNode(string nodeA, string nodeB, string config){
                context += nodeA + " -> " + nodeB +" [color="+color+", "+config+"];\n";
            }

            void dualConnectNode(string nodeA, string nodeB, string config){
                context += nodeA + " <-> " + nodeB +" [color="+color+", "+config+"];\n";
            }

              void simpleHorizontalConnectNode(string nodeA, string nodeB, string config){
                context += nodeA + ":e -> " + nodeB +":o [color="+color+", "+config+"];\n";
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
                        new_context+= nameNode + "[label=\"" + label +"\", shape=record, style=filled, fillcolor="+color+"];\n";
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

             void removeAllConections(string keep){
                stringstream separator(context);
                string temp;
                string new_context;

                while (getline(separator, temp, '\n')){
                    if (temp.find("->") != string::npos && temp.find(keep) == string::npos) {continue;}
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
            string output_file_tittle;
            string context;
            string config;
            SubgraphQueue queue_subgraph;

            void constructFile(const string filename) {
                ofstream outFile(filename);

                if (!outFile) return;

                outFile << context;
                outFile.close();
                //cout<<"Archivo: "<<filename<<"creado"<<endl;
            }

            int generateFile(){
                constructFile("graph"+tittle+".dot");

                string result_str = "dot -Tpng graph" + tittle + ".dot -o "+output_file_tittle+".png";
                //cout<<result_str<<endl;
                int result = system(result_str.c_str());
                return result;
            }

        public:

            DotFile(string tittle, string output_file_tittle): tittle(tittle), output_file_tittle(output_file_tittle) {}
            DotFile(string tittle, string output_file_tittle, string config): tittle(tittle), output_file_tittle(output_file_tittle), config(config) {}

            SubgraphQueue* getQueue() {return &queue_subgraph;}
            void resetContext() {
                context = "digraph "+tittle+" {}";
                generateFile();
            }

            int updateSubGraphs(){
                // rankdir=LR;
                if (context != "digraph "+tittle+" {\nlabel="+tittle+config +queue_subgraph.getContexts() +"\n}")
                {
                     context = "digraph "+tittle+" {\nlabel="+tittle+config + queue_subgraph.getContexts() +"\n}";
                     return 0;
                }
                return -1;

            }

            int generateNewFiles(){
                //queue_subgraph.print();

                return (updateSubGraphs() == 0) ? generateFile() : 2;

            }

    };


    /*PIXEL ARTS*/
    class PixelGraph{
        private:
            string tittle;
            string output_file_tittle;
            string context;
            string config;
            string subgraphs;

            void constructFile(const string filename) {
                ofstream outFile(filename);

                if (!outFile) return;

                outFile << context;
                outFile.close();
            }

            int generateFile(){
                constructFile("pixel_art_"+tittle+".dot");

                string result_str = "dot -Kneato -Tpng pixel_art_" + tittle + ".dot -o "+output_file_tittle+".png";
                int result = system(result_str.c_str());
                return result;
            }

        public:

            PixelGraph(string tittle, string output_file_tittle): tittle(tittle), output_file_tittle(output_file_tittle) {}


            void resetContext() {
                context = "digraph resultPixelArt {}";
                generateFile();
            }

            void addToContext(string st){
                subgraphs += st;
            }

            int updateSubGraphs(){
                string context_header = "graph resultPixelArt { \ngraph [splines=false]; \nnode [shape=square, width=1, height=1, label=\"\",fixedsize=true, margin=0, style=filled, fillcolor=white]; \nedge [style=invis];";
                if (context != context_header + subgraphs +"\n}")
                {
                    if(subgraphs == "") subgraphs = "subgraph{ non [fillcolor=black, color=none, pos=\"0,0!\"]}";

                    context = context_header + subgraphs +"\n}";
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
