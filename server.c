#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "include/server.h"
#include "include/protocol.h"

struct parameters_to_thread {
    int sock_client;
};

int main(int argc, char **argv)
{

    int sockfd, sock_client; // Listen on sockfd, new connection on sock_client
    int sin_size;
    struct sockaddr_in cliente; // Connector’s address information
    struct parameters_to_thread *param_thread;
    pthread_t tid; // Data types to store the data recived on a thread.

    sockfd = start_server();
    sin_size = sizeof(struct sockaddr_in);

    while ( (sock_client = accept(sockfd, (struct sockaddr *)&cliente, &sin_size) ) ){

        printf("Client acepted:\n");
        param_thread->sock_client = sock_client;

        if (pthread_create( &tid , NULL , connection_handler , param_thread) < 0){
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
    }

    perror("accept");
    exit(EXIT_FAILURE);

    return 0;
}

int start_server(){
    struct sockaddr_in server;
    int sockfd;

    printf("Starting server....\n");

    //create the socket.
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server)); //zero the rest of the struct
    server.sin_family = AF_INET; //host byte order
    server.sin_port = htons(MYPORT); //short, network byte order
    server.sin_addr.s_addr = INADDR_ANY; //automatically fill with my IP

    //Bind servidor.
    if((bind(sockfd, (struct sockaddr *)&server , sizeof(struct sockaddr))) == -1) {
        perror("Bind");
        exit(EXIT_FAILURE);
    }

    //Ponemos a las escucha el servidor un máximo de 5 conexiones concurrentes.
    if((listen (sockfd, BACKLOG)) == -1) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for incoming connections...\n");
    return sockfd;
}


void *connection_handler(void *param_thread)
{
    //Get the socket descriptor
    int sock_client;
    int total_read;
    int end = 1;
    struct parameters_to_thread *parameters;
    //char *message , client_message[MAXBUFFERSIZE];
    uint16_t code, len_message;
    char * message;
    char buffer[MAX_SIZE]; // Store the daya recived.

    parameters = (struct parameters_to_thread *) param_thread;
    sock_client = parameters->sock_client;

    //Receive a message from client
    while (end){

        total_read = parse_message(sock_client, &code, &len_message);

        if(total_read < 0){
            perror("recv");
            exit(EXIT_FAILURE);
            //puts("Client disconnected");
            //fflush(stdout);
        }

        printf("Total bytes read is: %d\n", total_read);
        printf("The code is: %u\n", code);

    }


    //Free the socket pointer
    //free(sock_client);

    return 0;
}