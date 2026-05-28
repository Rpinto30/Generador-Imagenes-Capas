<H1>MANUAL TECNICO</H1>
La aplicación se organizó en diferentes archivos .h de tal manera que cada uno cumpla con una función especifica y solo cumplan con su contrato. El archivo main es el principal y donde se utiliza todos los datos procesados por el resto de archivos, la estructuración fue la siguiente:

<H2>AUTOMATAS</H2>
Para la carga masiva se utilizaron 3 tipos de automatas diferentes: el automata_cap.h, automata_im.h y automata_usr.h; cada uno fue hecho exclusivamente para leer los archivos de la carpeta data y así, mapear los datos de estos archivos en una LinkedList propia que será explicada más adelante, esto para almacenarlos en memoria dinamica y poder agregar una cantidad indefinida de datos. A pesar que cada uno de los automatas realizan el analisis léxico con ayuda de Regex de la forma que muestra el siguiente diagrama de flujo:

DIAGRAMA_AUTOMATAS

<H2>DOTGENERATOR</H2>
Se creó un archivo titulado DotGenerator.h encargado de administrar el uso de graphviz para la generación de imagenes, tanto de memoria como los resultados pixelArt. 
El DotFile es una clase que se encarga de formar un archivo .dot con la información necesaria para generar lo requerido, en el caso de las imagenes de la gestión de memoria se utiliza una cola (también propiamente hecha) de clases auxiliares llamadas Subgraphs, en un archivo .dot se parte por crear instancia de un digraph que englobe todo lo dibujado y un subgraph es la manera en la cual se agrupan las diferentes estructuras de datos, la cola de Subgraphs sirve para guardar las referencias y no copias de los subgraphs que se le son asignados a todas las estructuras de datos, un subgraph almacena en una variable string denominada context la información de los nodos, conexiones y configuraciones de la imagen. Luego el DotFile itera sobre cada una de estas referencias a los subgraphs, concatenando sus context al contexto propio, para luego llamar por medio del metodo system() al comando para generar la imagen en memoria.
Para el caso de las imagenes pixelArt, se utilizo el mismo flujo, cambiando el aspecto que ahora cada capa crea su propio context especial para guardar la posición y el color del pixel iterando sobre la matriz dispersa, ya no se utiliza el DotFile, sino la clase llamada PixelGraph, la cual realiza casi las mismas acciones que un DotFile, con el distintivo que utiliza el motor Kneato para permitir la instancia de nodos (los cuales actuan de pixeles) con posición a base de coordenadas, vuelve a repetir el proceso de un DotFile, con la caracteristica que reemplaza la cola se Subgraphs por un método que contatena el context de la capa que queremos agregar a la imagen.

Para evitar el exceso de archivos y fomentar una mayor limpieza en las salidas del sistema, se optó por eliminar el archivo .dot luego que este haya sido usado por el metodo system(), dejando así solo el resultado de la imagen generada. La clase DotFile como en su mayoria el archivo DotGenerator.h siguen el siguiente diagrama de flujo:

DIAGRAMA DOTGENERATOR

<H2>STRUCTS Y DEFINITIONS</H2>
Para almacenar toda la información de la aplicación, se crearon desde 0 las siguientes estructuras de datos:
- Lista enlazada
- Lista circular doble enlazada
- Cola
- Arbol binario de busqueda
- Matriz dispersa 

Al ser estructuras de datos que se iban a utilizar en multiples lugares, se optó por realizar clases genericas, ayudandose que el lenguaje de C++ incluye la opción de implementarlas por medio de template. En el archivo structs.h se encontrarán la declaración de todas las estructuras de datos como la declaración de los TDA utilizados en el sistama.
En el archivo definition.h se encuentra la declación ya de estas estructuras con el tipo que le corresponde almacenar, además de la declaración de diversos metodos que son utilizados en el main.cpp para su utilización.

<H2>DIAGRAMA DE FLUJO GENERAL</H2>
DIAGRAMA GENERAL