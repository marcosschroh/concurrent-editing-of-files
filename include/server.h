#define MAXDAT 65535
#define MAXNOM 255
#define MAXCLI 5


int conectar();
void mostrar_datos(u_int16_t, u_int16_t, u_int16_t, u_int16_t, u_int16_t, char*);
int cargar_mensaje(int, u_int16_t*, u_int16_t*, u_int16_t*, u_int16_t*, u_int16_t*, char*, char*);
void enviar(int, u_int16_t, u_int16_t, u_int16_t, u_int16_t, u_int16_t, char*);
u_int16_t ultimo_id(char*);

void crear_archivo(int, char*);
void eliminar_archivo(int, char*);
void sincronizar_archivo(int, char*);
void conectar_archivo(int, char*);
void editar_archivo(int, u_int16_t, u_int16_t, u_int16_t, u_int16_t, char*);
void agregar_usuario(int, char*);
char* ver_archivo_usuario(int);