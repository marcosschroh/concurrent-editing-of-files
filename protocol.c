#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h> //uint16_t

#include "include/protocol.h"


int read_n_bytes(int sock_client, void *buffer, int len_to_read){
    int total_read;

    total_read = recv(sock_client, buffer, len_to_read, 0);

    return total_read;
}

int parse_message(int sock_client, uint16_t *code){
    int total_read;
    uint16_t code_message;

    total_read = read_n_bytes(sock_client, &code_message, HEADER_CODE_LENGTH);
    *code = ntohs(code_message);

    return total_read;
}


