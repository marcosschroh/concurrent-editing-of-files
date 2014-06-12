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
    int operation, l; //Variable utilizada para el ciclo
    char* data; //variable utilizada para alamcenar los datos
    uint16_t code, size_message;
    char buffer[TOTAL_SIZE];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    sockfd = start_client(argv[1]);
    operation = 0;

    while(1) {

        data = malloc(MAX_DATA_SIZE);
        memset(data, 0, MAX_DATA_SIZE);
        printf("Enter a message\n");
        scanf("%s", data);
        code = htons(100);

        size_message = htons(strlen(data));
        printf("Data: %s\n", data);
        printf("Total size message: %u\n", size_message );
        memset(buffer, 0, TOTAL_SIZE);
        memcpy(buffer, &code, CODE_LENGTH);
        memcpy(buffer + CODE_LENGTH, &size_message , MESSAGE_LENGTH);
        memcpy(buffer + HEADER_LENGTH, data, strlen(data));

        printf("Size data %d\n", strlen(data));

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
    printf("IP Address is: %s\n", inet_ntoa(*((struct in_addr *)he->h_addr)));

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