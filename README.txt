Proyecto para editar archivos de forma concurrente en lenguaje C.
En este proyecto se trata de demostrar el uso de sockets y no la edicion de los archivos en si.C

Cada vez que editamos un archivo, se agrega contenido al FINAL!!

PASOS:

1- Compilar el SERVIDOR:
    En el directorio server_dir ejecutar make

2- Ejecutar el SERVIDOR:
    En el directorio server_dir ejecutar ./server

3- Compilar el CLIENTE:
    En el directorio cliente_dir ejecutar make

4- Ejecutar el CLIENTE:
    En el directorio cliente_dir ejecutar ./cliente IP_SERVIDOR


EXTRA:

Para remover los archivos y complilar de nuevo hacer
make clean (ya sea en el directorio server_dir o cliente_dir)

Los archivos en el servidor se almacenan en el directorio files_store.

Los archivos descargados en el cliente se almacenan en el directorio files_store.
