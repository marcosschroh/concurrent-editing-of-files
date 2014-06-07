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
    int operation; //Variable utilizada para el ciclo
    char* data; //variable utilizada para alamcenar los datos
    //char *server_reply;
    uint16_t code, size;
    char buffer[HEADER_CODE_LENGTH + MAXDATASIZE];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    sockfd = start_client(argv[1]);
    operation = 0;

    while(1) {

        data = malloc(MAXDATASIZE);
        memset(data, 0, MAXDATASIZE);
        printf("Enter a message\n");
        scanf("%s", data);
        code = htons(100);
        printf("Size of code: %d\n", sizeof(code));
        printf("The code is:  %u\n", code);
        printf("The code is:  %d\n", ntohs(code));

        memset(buffer, 0, HEADER_CODE_LENGTH + MAXDATASIZE);
        memcpy(buffer, &code, HEADER_CODE_LENGTH);

        if ( (send(sockfd, buffer, HEADER_CODE_LENGTH, 0)) == -1){
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