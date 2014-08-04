#define MAXNOM 255
#define MYPORT 3490

#define FILE_DOESNT_EXIST "File don't exist"

int start_client(char *);
void show_options(int *);
void send_message(int, uint16_t, char* );
void listen_server(int);
