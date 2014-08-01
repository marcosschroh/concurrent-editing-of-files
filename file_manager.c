#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h> //uint16_t
#include <dirent.h> // to list files in a DIR
#include <fcntl.h> //

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
    //FILE* archivo;
    int pfd; /* Integer for file descriptor returned by open() call. */
    char* file_path;
    struct flock lock;

    file_path = get_file_path(file_name);

    if((pfd = open(file_path, O_WRONLY)) != -1){

        /* Initialize the flock structure.  */
        memset(&lock, 0, sizeof(lock));

        lock.l_type = F_WRLCK;

        printf ("Locked the file %s...\n\n",file_path);

        /* Place a write lock on the file.  */
        fcntl(pfd, F_SETLKW, &lock);

        //printf("Data recived: %s\n\n", data);

        //The sleep is use to test the concurrent editing!!!
        //sleep(25);

        printf("Editing...\n\n");

        lseek(pfd, 0, SEEK_END);
        write(pfd, " ", 1);
        write(pfd, data, strlen(data));

        /* Release the lock.  */
        lock.l_type = F_UNLCK;
        fcntl(pfd, F_SETLKW, &lock);

        printf ("Unlocking\n\n");

        close(pfd);
    }

    //printf("The file name is: %s\n", file_path);
    //printf("The data is: %s\n", data);
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