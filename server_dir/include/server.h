#define MAXNOM 255
#define BACKLOG 10
#define MYPORT 3490
#define LENGHT_LOCKS 2

int start_server();
void *connection_handler(void *);
int create_dir();
void crear_archivo(int, u_int16_t, char[]);
void send_message(int, uint16_t, char[]);
//int lock_file(struct Lock [], char[]);