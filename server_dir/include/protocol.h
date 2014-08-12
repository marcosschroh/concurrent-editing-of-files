/*              Protocol
##############################################
#      #       #                             #
# code #  size #           DATA              #
#      #       #                             #
##############################################
*/

// max data size
#define MAX_DATA_SIZE 65535

// Protocolo Header
#define HEADER_CODE_LENGTH 2 // use uint16_t
#define HEADER_SIZE_LENGTH 2 // use uint16_t
#define HEADER_TOTAL_LENGTH HEADER_CODE_LENGTH + HEADER_SIZE_LENGTH

//Protocol size
#define TOTAL_SIZE HEADER_TOTAL_LENGTH + MAX_DATA_SIZE

// Codes
#define CREATE_FILE 100
//#define DELETE_FILE 200 //NOT IMPLEMENTED
#define FILE_LIST   300
#define KEEP_FILE   400
#define UPDATE_FILE 500
#define DOWNLOAD_FILE 600
#define EXIT        700

//CUSTOM ERROR MESSAGES
#define ERROR_FILE_ALREADY_EXISTS "The File already exist"
#define ERROR_FILE_DOESNT_EXIST "File don't exist"
#define ERROR_FILE_KEPT_FIRST "You have to keep the a file first"
#define ERROR_FILE_EDITING_BY_OTHER "The file is being editing by other user"

//CUSTOM SUCESS MESSAGES
#define SUCCESS_FILE_CREATED "File created with success"
#define SUCCESS_FILE_KEPT "File kept with success"
#define SUCCESS_FILE_EDITED "File edited with success"


int read_n_bytes(int, void *, int);
int parse_message(int, uint16_t *, uint16_t *, char []);