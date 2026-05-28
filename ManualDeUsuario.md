<H1>MANUAL DE USUARIO</H1>
La aplicación consiste en un generador de imagenes a partir de capas pre-establecidas antes de ejecutar la aplicación.
En la carpeta de ruta "GeneratorImages/data" encontraras 3 archivos:
 -capa.cap
 -imagenes.im
 -usuarios.usr

Estos archivos sirven para cargar datos previo a la ejecución de la aplicación, puedes cargar la cantidad de datos que desees, pero debes seguir el siguiente formato;
* <b><H3>ARCHIVO capas.cap</H3></b>
id_unico {
    fila_pixel, columna_pixel, color;
}

* <b><H3>ARCHIVO imagenes.img</H3></b>
id_unico {
    id_capa_existente, ..., id_capa_existente
}

* <b><H3>ARCHIVO usuarios.usr</H3></b>
nombreUsuario: id_imagen_existente, ..., id_capa_existente;

<H2>INTERFAZ Y MENÚ PRINCIPAL</H2>
![menu_principal] (ImgManual/menu_principal.png)

El menú principal lista las opciones mostradas, catalogando cada opción según su funcionalidad, si deseas salir del sistema puedes seleccionar 0 en el menú principal, 0 es el caracter por defecto para salir del menú actual. El sistema sigue la siguiente simbologia:

* !: Información de evento provocado por el sistema
* x: Fallo en algun apartado del sistema
* >: Solicitud de entrada por parte del usuario

<H2>OPCIONES DE USUARIO</H2>
![menu_usuario] (ImgManual/menu_usuario.png)

El menú presenta las opciones principales para gestionar los usuario, a continuación se describe cada una de las opciones:
* <b><H3>1) CREAR UN NUEVO USUARIO</H3></b>
Permite crear un nuevo usuario, solicitando unicamente un nombre de usuario, el id se agrega automatico autoincremental.

* <b><H3>2) VER USUARIOS</H3></b>
Muestra todos los usuarios que están registrados en el sistema siguiendo el formato "ID: | Usuario:".

* <b><H3>3) ELIMINAR USUARIO</H3></b>
Se solicita un ID de usuario existente y se remueve del sistema.

* <b><H3>4) MODIFICAR USUARIO</H3></b>
Se solicita un ID de usuario existente y luego se solicita un nuevo nombre para el usuario seleccionado.

* <b><H3>5) VER IMAGENES DE USUARIO</H3></b>
Lista todas las imagenes que el usuario tiene registradas, siguiendo el formato "Imagen: ".

* <b><H3>6) AGREGAR IMAGEN A USUARIO</H3></b>
Se solicita un ID de usuario y luego se listan todas las imagenes que están registradas en el sistema, se solicita seleccionar un ID de imagen y luego se le asigna esa imagen al usuario, una imagen puede ser asignada a varios usuarios, pero la asignación es unica, por lo que no se puede asignar una imagen más de una vez.

* <b><H3>7) ELIMINAR IMAGEN DE USUARIO</H3></b>
Se solicita el ID de un usuario y luego el ID de una de las imagenes que posee el usuario, si el ID no existe entre las imagenes del usuario el sistema solo lo rechazará. Esta opción no elimina la imagen del sistema, solo le quita la dependencia al usuario de la imagen.

<H2>GENERACIÓN DE IMAGENES</H2>
![menu_imagenes] (ImgManual/menu_imagenes.png)

En este apartado podemos generar las imagenes pixel art, todos los resultado son almacenados en formato .png en la carpeta de ruta "GeneratorImages/ResultImages", a continuación se listan las opciones mostradas:

* <b><H3>1) GENERAR IMAGEN POR USUARIO</H3></b>
Se solicita el ID de un usuario registrado al sistema, luego, se listan las imagenes que tiene asignado a su nombre, solicitandole al usuario ingresar el ID de la imagen que desea generar, una vez seleccionado se comenzará a generar, la forma en la que se agregarn las capas de la imagane es siguiendo un método FIFO (First In First Out), el sistema informa el orden en el que se generan las capas.

* <b><H3>2) GENERAR IMAGEN POR CAPAS</H3></b>
Se listan y se le solicita al usuario ingresar el ID de una capa ya registrada al sistema, luego el sistema procederá a generar la capa única.

* <b><H3>3) GENERAR POR RECORRIDO</H3></b>
Las capas se guardan en un ARBOL BINARIO DE BUSQUEDA, el cual se puede recorrer por completo de 3 formas: postorden, inorden y preorden. El sistema hara elegir al usuario sobre alguna de estas tres opciones, una vez seleccionada el sistema seleccionará las capas según el recorrido y generará la imagen.

<H2>GESTIONAR LA MEMORIA</H2>
![menu_memoria] (ImgManual/menu_memoria.png)

En este apartado se dan las opciones al usuario para gestionar la memoria que está utilizando la aplicación, esto se hace por medio de la herramienta graphviz, la cual genera imagenes en la carpeta de ruta "GeneratorImages/Memory", a continuación se listan las opciones mostradas:


* <b><H3>1) MOSTRAR LISTA CIRCULAR DOBLE DE IMAGENES</H3></b>
Se genera una imagen que muestra como se almacenan las imagenes en el sistema, además se muestra como se alamacenan todas las capas que posee una imagen.

* <b><H3>2) MOSTRAR ARBOL DE CAPAS</H3></b>
Muestra el ARBOL BINARIO DE BUSQUEDA en el cual se almacenan las capas del sistema.

* <b><H3>3) MOSTRAR CAPA</H3></b>
Se le solicita al usuario el ID de una capa previamente insertada al sistema, luego muestra un grafico tipo matriz, donde se representan como nodos todos los pixeles de la capa seleccionado; Muestra las filas, columnas y el color de cada pixel.

* <b><H3>4) MOSTRAR IMAGEN Y ARBOL DE CAPAS</H3></b>
Se solicita al usuario el ID de una imagen previamnte insertada al sistema, luego se genera el nodo de la imagen seleccionada junto a las capas que utiliza, esto es acompañado con el arbol binario de busqueda, donde se muestran las dependencias de ambas estructuras utilizadas.

* <b><H3>5) MOSTRAR ARBOL DE USUARIOS</H3></b>
Muestra el ARBOL BINARIO DE BUSQUEDA en el cual se almacenan los usuarios del sistema.





