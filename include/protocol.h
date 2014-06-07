/*              Protocol
##############################################
#      #       #                             #
# code #  size #           DATA              #
#      #       #                             #
##############################################
*/

// max size
#define MAX_SIZE 65535

// Protocolo Header
#define HEADER_CODE_LENGTH 2 // use uint16_t
#define HEADER_SIZE_LENGTH 2 // use uint16_t

// Codes
#define CREATE_FILE 100
#define DELETE_FILE 200
#define FILE_LIST   300
#define SEND_DATA   400
#define RECIVE_FILE 500
#define EXIT        600

int read_n_bytes(int, void *, int);
int parse_message(int, uint16_t *);