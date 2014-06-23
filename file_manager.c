#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h> //uint16_t

#include "include/protocol.h"
#include "include/file_manager.h"

void create_file(int sock, u_int16_t size_message, char message_recive[]){
    char* response;
    char* file_path;
    FILE* archivo;

    file_path = malloc(12);
    memset(file_path, 0, 12);

    response = malloc(MAX_DATA_SIZE);
    memset(response, 0, MAX_DATA_SIZE);

    strcpy(file_path, FILES_STORE_DIR);
    strcat(file_path, "/");
    strcat(file_path, message_recive);
    printf("The file name is: %s\n", file_path);

    if((archivo = fopen(file_path, "r")) == NULL){
        archivo = fopen(file_path, "w");
        fclose(archivo);
        //id = 1;
        //datos = "El archivo fue creado.";
        printf("Archivo creado\n");
    }else{
        printf("El Archivo ya existe\n");
            //datos = "Ya existe un archivo con ese nombre.";
    }
        //longitud = strlen(datos);

        //enviar(sock, 1, id, 0, 0, longitud, datos);
}
