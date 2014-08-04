#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<netdb.h>    //hostent
#include <sys/stat.h>

#include "include/cliente.h"
#include "include/protocol.h"
#include "include/file_manager.h"

//VARIABLES GLOBALES
char* file_to_download; //variable almacenada para saber que archivo vamos a bajar

int main(int argc, char **argv)
{
    int sockfd; //File Descriptor del Socket.
    int option=0; //Variable utilizada para el ciclo
    char* data; //variable utilizada para alamcenar los datos
    uint16_t code=0;

    if (argc != 2) {
        fprintf(stderr,"usage: give the server ip\n");
        exit(1);
    }

    sockfd = start_client(argv[1]);
    data = malloc(MAX_DATA_SIZE);
    file_to_download = malloc(50);
    memset(file_to_download, 0, 50);

    while(code != htons(EXIT) ) {

        memset(data, 0, MAX_DATA_SIZE);
        show_options(&option);

        switch (option) {

            case 1:
                code = htons(CREATE_FILE);
                printf("Enter the name of the file\n");
                scanf("%s", data);
                send_message(sockfd, code, data);
                break;
            case 2:
                code = htons(200);
                break;

            case 3:
                code = htons(FILE_LIST);
                send_message(sockfd, code, data);
                break;

            case 4:
                code = htons(KEEP_FILE);
                printf("Enter the name of the file\n");
                scanf("%s", data);
                send_message(sockfd, code, data);
                break;

            case 5:
                code = htons(UPDATE_FILE);
                printf("Enter the data to edit the file kept\n");
                scanf("%s", data);
                send_message(sockfd, code, data);
                break;

            case 6:
                code = htons(DOWNLOAD_FILE);
                printf("Enter the name of the file to download\n");
                scanf("%s", data);
                file_to_download = data;
                send_message(sockfd, code, data);
                break;

            case 7:
                code = htons(EXIT);
                send_message(sockfd, code, data);
                break;

            default:
                printf("El número ingresado no concuerda con ninguna opción.\n");
                break;
        }

        listen_server(sockfd);
        memset(data, 0, strlen(data));
    }

    return 0;
}

int start_client(char *server_ip){

    struct sockaddr_in server; // server’s address information
    int sockfd, i;
    struct hostent *he;
    struct in_addr **addr_list;

    printf("Iniciando el cliente...\n");

    //creating dir to store the downloads
    create_dir_downloads();

    //if ( (he=gethostbyname(server_name) ) == NULL ) {
        ////perror("gethostbyname");
        //exit(1);
    //}

    //printf("resolved to : %s\n" , server_name);
    //printf("IP Address is: %s\n\n", inet_ntoa(*((struct in_addr *)he->h_addr)));

    //Create the socket.
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(EXIT_FAILURE);
        }

        memset(&server, 0, sizeof(server)); // zero the rest of the struct
        server.sin_family = AF_INET; // host byte order
        server.sin_port = htons(MYPORT); // short, network byte order
        //server.sin_addr = *((struct in_addr *)he->h_addr);
        server.sin_addr.s_addr = inet_addr(server_ip);

        //Conectamos con el servidor.
        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
            perror("Connect");
            exit(EXIT_FAILURE);
        }

        return sockfd;
}

void show_options(int *option){

    printf("####OPTIONS####\n");
    printf("\n");
    printf("1- Crear archivo.\n");
    printf("2- Borrar archivo.\n");
    printf("3- Solicitar lista.\n");
    printf("4- Keep archivo.\n");
    printf("5- Enviar datos.\n");
    printf("6- Descargar archivo.\n");
    printf("7- Para salir.\n\n");
    printf("Elija opción: \n");
    scanf("%d", option);
    printf("\n");

}

void send_message(int sockfd, uint16_t code, char* data){
    char buffer[TOTAL_SIZE];
    uint16_t size_message=0;

    size_message = htons(strlen(data));
    //printf("Data: %s\n", data);
    //printf("Total size message: %u\n", size_message );
    memset(buffer, 0, TOTAL_SIZE);
    memcpy(buffer, &code, HEADER_CODE_LENGTH);
    memcpy(buffer + HEADER_SIZE_LENGTH, &size_message , HEADER_SIZE_LENGTH);
    memcpy(buffer + HEADER_TOTAL_LENGTH, data, strlen(data));
    //printf("Size data %d\n", strlen(data));

    if ( (send(sockfd, buffer, HEADER_TOTAL_LENGTH + strlen(data), 0)) == -1){
        perror("send");
        exit(EXIT_FAILURE);
    }

}

void listen_server(int sockfd){
    int total_read;
    char message_recive[MAX_DATA_SIZE]="\0";
    uint16_t code=0, size_message=0;

    total_read = parse_message(sockfd, &code, &size_message, message_recive);

    printf("####RESPONSE####\n");

    switch (code){
        case CREATE_FILE:
            printf("Data Recived: %s\n", message_recive);
            break;

        case FILE_LIST:
            printf("Files stored in the server: %s\n", message_recive);
            break;

        case KEEP_FILE:
            printf("%s\n", message_recive);
            break;

        case UPDATE_FILE:
            printf("%s\n", message_recive);
            break;

        case DOWNLOAD_FILE:
            create_file(file_to_download);
            if ( update_file(file_to_download, message_recive) == 1){
                printf("The file %s was downloaded...\n", file_to_download);
                break;
            }

            printf("The file %s cannot be downloaded...\n", file_to_download);
            break;

        case EXIT:
            printf("%s\n", message_recive);
            close(sockfd);
            break;

        default:
            printf("Invalid Option\n");
            break;
    }

    printf("\n");
    //printf("Total bytes read is: %d\n", total_read);

}


int create_dir_downloads()
{
    struct stat info;

    if ( mkdir(FILES_STORE_DIR, 0777) == 0){
        printf("Creating directory to downloads the files....\n");
    }

    return 1;
}