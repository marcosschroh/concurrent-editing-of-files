#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h> //uint16_t
#include <dirent.h> // to list files in a DIR

#include "include/protocol.h"
#include "include/file_manager.h"

char* get_file_path(char file_name[]){

    char* file_path;

    file_path = malloc(strlen(FILES_STORE_DIR ));
    memset(file_path, 0, strlen(FILES_STORE_DIR ));

    strcpy(file_path, FILES_STORE_DIR);
    strcat(file_path, "/");
    strcat(file_path, file_name);

    return file_path;

}

int create_file(char file_name[]){
    char* file_path;
    FILE* archivo;

    file_path = get_file_path(file_name);

    if((archivo = fopen(file_path, "r")) == NULL){
        archivo = fopen(file_path, "w");
        fclose(archivo);
        return 1;
    }

    return 0;
}

int file_exist(char file_name[]){

    char* file_path;

    file_path = get_file_path(file_name);

    if((fopen(file_path, "r")) == NULL){
        return 0;
    }

    return 1;
}


int update_file(char file_name[], char data[]){
    FILE* archivo;
    char* file_path;

    file_path = get_file_path(file_name);

    if((archivo = fopen(file_path, "a")) != NULL){
        fprintf(archivo, " ");
        fprintf(archivo, data);
        fclose(archivo);
    }

    printf("The file name is: %s\n", file_path);
    printf("The data is: %s\n", data);
    return 1;
}

void list_files(char array_list[]){
    DIR* d;
    struct dirent *dir;
    d = opendir(FILES_STORE_DIR);

    if (d){
        while ((dir = readdir(d)) != NULL){
          //printf("%s\n", dir->d_name);
          //printf("%s\n", dir->d_type);
          strcat(array_list, dir->d_name);
          strcat(array_list, " ");
        }

    closedir(d);
  }

}