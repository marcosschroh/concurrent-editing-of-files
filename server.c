#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>

#include "include/server.h"
#include "include/protocol.h"
#include "include/file_manager.h"

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


    create_dir();

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
    struct parameters_to_thread *parameters;
    //char *message , client_message[MAXBUFFERSIZE];
    uint16_t code=0, size_message=0;
    char buffer[TOTAL_SIZE]="\0", message_recive[MAX_DATA_SIZE]="\0"; // Store the daya recived.
    char message_send[MAX_DATA_SIZE]="\0";
    char file_kept[MAX_DATA_SIZE]="\0";

    parameters = (struct parameters_to_thread *) param_thread;
    sock_client = parameters->sock_client;

    //Receive a message from client
    while (code != EXIT){

        total_read = parse_message(sock_client, &code, &size_message, message_recive);

        if(total_read < 0){
            perror("recv");
            exit(EXIT_FAILURE);
            //puts("Client disconnected");
            //fflush(stdout);
        }

/*
        printf("The code is: %u\n", code);
        printf("The message size is: %u\n", size_message);
        printf("The data is: %s\n", message_recive);
        printf("Total bytes read is: %d\n", total_read);
*/
        memset(message_send, 0, MAX_DATA_SIZE);

        switch (code){
            case CREATE_FILE:
                strcpy(message_send, "The File already exist");
                if (create_file(sock_client, size_message, message_recive) == 1){
                    strcpy(message_send, "File created with success");
                }
                send_message(sock_client, htons(CREATE_FILE), message_send);
                break;

            case KEEP_FILE:
                strcpy(file_kept, message_recive);
                strcpy(message_send, "File kept with sucess");
                send_message(sock_client, htons(KEEP_FILE), message_send);
                break;

            case FILE_LIST:
                list_files(message_send);
                send_message(sock_client, htons(FILE_LIST), message_send);
                break;

            case EXIT:
                printf("Bye\n");
                strcpy(message_send, "Close program....");
                send_message(sock_client, htons(EXIT), message_send);
                break;
        }

        memset(message_recive, 0, MAX_DATA_SIZE);
        memset(message_send, 0, MAX_DATA_SIZE);
    }


    //close the socket
    close(sock_client);
}

int create_dir()
{
    struct stat info;

    if ( mkdir(FILES_STORE_DIR, 0777) == 0){
        printf("Creating directory to store the share files....\n");
    }

    return 1;
}

void send_message(int sockfd, uint16_t code, char data[]){
    char buffer[TOTAL_SIZE];
    uint16_t size_message=0;

    size_message = htons(strlen(data));
    memset(buffer, 0, TOTAL_SIZE);
    memcpy(buffer, &code, HEADER_CODE_LENGTH);
    memcpy(buffer + HEADER_SIZE_LENGTH, &size_message , HEADER_SIZE_LENGTH);
    memcpy(buffer + HEADER_TOTAL_LENGTH, data, strlen(data));

    if ( (send(sockfd, buffer, HEADER_TOTAL_LENGTH + strlen(data), 0)) == -1){
        perror("send");
        exit(EXIT_FAILURE);
    }

}