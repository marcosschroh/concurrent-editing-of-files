#define MAXBUFFERSIZE 65535
#define MAXNOM 255
#define BACKLOG 10
#define MYPORT 3490

int start_server();
void *connection_handler(void *);