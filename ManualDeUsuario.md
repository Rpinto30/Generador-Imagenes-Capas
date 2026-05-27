<H1>MANUAL DE USUARIO</H1>
La aplicación consiste en un generador de imagenes a partir de capas pre-establecidas antes de ejecutar la aplicación.
En la carpeta de ruta "GeneratorImages/data" encontraras 3 archivos:
 -capa.cap
 -imagenes.im
 -usuarios.usr

Estos archivos sirven para cargar datos previo a la ejecución de la aplicación, puedes cargar la cantidad de datos que desees, pero debes seguir el siguiente formato;
* <H2>ARCHIVO capas.cap</H2>
id_unico {
    fila_pixel, columna_pixel, color;
}

* <H2>ARCHIVO imagenes.img</H2>
id_unico {
    id_capa_existente, ..., id_capa_existente;
}

* <H2>ARCHIVO usuarios.usr</H2>
nombreUsuario: id_imagen_existente, ..., id_capa_existente;

<H2>INTERFAZ Y MENÚ PRINCIPAL</H2>
![menu_principal] (ImgManual/menu_principal.png)

<H2>OPCIONES DE USUARIO</H2>
<H2>GENERACIÓN DE IMAGENES</H2>
<H2>GESTIONAR LA MEMORIA</H2>