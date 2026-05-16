#include <iostream>
#include "structs.h"
// #include "DotGenerator.h"

using namespace std;
using namespace st;

int main()
{
    Layer ly;
    ly.insert(1,1, "#111111");
    ly.insert(2,1, "#222222");
    ly.insert(3,4, "#333333");
    ly.insert(6,2, "#444444");
    ly.insert(2,4, "#555555");
    ly.insert(3,3, "#666666");

    return 0;
}


// Graphviz
// Mermaid.js
