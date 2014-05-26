#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/cliente.h"

int main(int argc, char **argv)
{
    int sockfd; //File Descriptor del Socket.
    int operation; //Variable utilizada para el ciclo
    char message[MAXDATASIZE] = "\0";
    //char *server_reply;

    sockfd = start_client();
    operation = 0;

    while(1) {

        printf("Enter a message\n");
        scanf("%s", &message);

        if ( (send(sockfd , message , strlen(message) , 0)) == -1){
            perror("send");
            exit(EXIT_FAILURE);
        }
        printf("Sending the message:%s with total bytes: %d \n\n", message, strlen(message));
        memset(message, 0, strlen(message));
    }

    return 0;
}

int start_client(){

    struct sockaddr_in server; // server’s address information
    int sockfd;

    printf("Iniciando el cliente...\n");

    //Create the socket.
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(EXIT_FAILURE);
        }

        memset(&server, 0, sizeof(server)); // zero the rest of the struct
        server.sin_family = AF_INET; // host byte order
        server.sin_port = htons(MYPORT); // short, network byte order
        server.sin_addr.s_addr = INADDR_ANY;

        //Conectamos con el servidor.
        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
            perror("Connect");
            exit(EXIT_FAILURE);
        }
        return sockfd;
}