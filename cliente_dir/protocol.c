#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h> //uint16_t

#include <string.h>

#include "include/protocol.h"


int read_n_bytes(int sock_client, void *buffer, int len_to_read){
    int total_read=0;

    total_read = recv(sock_client, buffer, len_to_read, 0);

    return total_read;
}

int parse_message(int sock_client, uint16_t *code, uint16_t *size, char message[]){
    int total_read=0, bytes_read=0;
    uint16_t code_message, size_message;
    char* data;

    //get operation code
    bytes_read = read_n_bytes(sock_client, &code_message, HEADER_CODE_LENGTH);
    total_read += bytes_read;
    *code = ntohs(code_message);

    //get data size
    bytes_read = read_n_bytes(sock_client, &size_message, HEADER_SIZE_LENGTH);
    *size = ntohs(size_message);
    total_read += bytes_read;

    if(*size > 0){
        bytes_read = read_n_bytes(sock_client, message, ntohs(size_message));
        total_read += bytes_read;
    }

    return total_read;
}


