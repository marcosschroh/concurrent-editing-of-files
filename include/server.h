#define MAXNOM 255
#define BACKLOG 10
#define MYPORT 3490

int start_server();
void *connection_handler(void *);
int create_dir();
void crear_archivo(int, u_int16_t, char[]);