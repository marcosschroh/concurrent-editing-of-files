#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<netdb.h>    //hostent

#include "include/cliente.h"

int main(int argc, char **argv)
{
    int sockfd; //File Descriptor del Socket.
    int option=0; //Variable utilizada para el ciclo
    char* data; //variable utilizada para alamcenar los datos
    uint16_t code=0, size_message=0;
    char buffer[TOTAL_SIZE];

    if (argc != 2) {
        fprintf(stderr,"usage: give a Client Hostname\n");
        exit(1);
    }

    sockfd = start_client(argv[1]);
    data = malloc(MAX_DATA_SIZE);

    while(code != htons(600) ) {

        memset(data, 0, MAX_DATA_SIZE);
        show_options(&option);

        switch (option) {

            case 1:
                code = htons(100);
                printf("Enter the name of the file\n");
                scanf("%s", data);

                size_message = htons(strlen(data));
                printf("Data: %s\n", data);
                printf("Total size message: %u\n", size_message );
                memset(buffer, 0, TOTAL_SIZE);
                memcpy(buffer, &code, CODE_LENGTH);
                memcpy(buffer + CODE_LENGTH, &size_message , MESSAGE_LENGTH);
                memcpy(buffer + HEADER_LENGTH, data, strlen(data));
                printf("Size data %d\n", strlen(data));
                break;
            case 2:
                code = htons(200);
                break;

            case 3:
                code = htons(300);
                break;

            case 4:
                code = htons(400);
                break;

            case 5:
                code = htons(500);
                break;

            case 6:
                code = htons(600);
                size_message = htons(strlen(data));
                memset(buffer, 0, TOTAL_SIZE);
                memcpy(buffer, &code, CODE_LENGTH);
                memcpy(buffer + CODE_LENGTH, &size_message , MESSAGE_LENGTH);
                memcpy(buffer + HEADER_LENGTH, data, strlen(data));
                break;

            default:
                printf("El número ingresado no concuerda con ninguna opción.\n");
                break;
        }

        if ( (send(sockfd, buffer, HEADER_LENGTH + strlen(data), 0)) == -1){
            perror("send");
            exit(EXIT_FAILURE);
        }

        memset(data, 0, strlen(data));
    }

    return 0;
}

int start_client(char *server_name){

    struct sockaddr_in server; // server’s address information
    int sockfd, i;
    struct hostent *he;
    struct in_addr **addr_list;

    printf("Iniciando el cliente...\n");

    if ( (he=gethostbyname(server_name) ) == NULL ) {
        perror("gethostbyname");
        exit(1);
    }

    printf("resolved to : %s\n" , server_name);
    printf("IP Address is: %s\n\n", inet_ntoa(*((struct in_addr *)he->h_addr)));

    //Create the socket.
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(EXIT_FAILURE);
        }

        memset(&server, 0, sizeof(server)); // zero the rest of the struct
        server.sin_family = AF_INET; // host byte order
        server.sin_port = htons(MYPORT); // short, network byte order
        server.sin_addr = *((struct in_addr *)he->h_addr);

        //Conectamos con el servidor.
        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
            perror("Connect");
            exit(EXIT_FAILURE);
        }
        return sockfd;
}

void show_options(int *option){

    printf("\n");
    printf("1- Crear archivo.\n");
    printf("2- Borrar archivo.\n");
    printf("3- Solicitar lista.\n");
    printf("4- Enviar datos.\n");
    printf("5- Recibir datos.\n");
    printf("6- Para salir.\n\n");
    printf("Elija opción: \n");
    scanf("%d", option);
    printf("\n");

}