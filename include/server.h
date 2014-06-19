#define MAXNOM 255
#define BACKLOG 10
#define MYPORT 3490
#define FILES_STORE_DIR "files_store"

int start_server();
void *connection_handler(void *);
int create_dir();