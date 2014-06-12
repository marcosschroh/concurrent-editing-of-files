#define MAX_DATA_SIZE 65535
#define MAXNOM 255
#define MYPORT 3490
#define CODE_LENGTH 2 // use uint16_t
#define MESSAGE_LENGTH 2 // use uint16_t
#define HEADER_LENGTH CODE_LENGTH + MESSAGE_LENGTH
#define TOTAL_SIZE CODE_LENGTH + MESSAGE_LENGTH + MAX_DATA_SIZE

int start_client(char *);
