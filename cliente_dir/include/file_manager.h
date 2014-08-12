#define FILES_STORE_DIR "files_store"

int create_file(char []);
void list_files();
int update_file(char [], char []);
int file_exist(char []);
char* get_file_path(char[]);
void get_file(char [], char[]);