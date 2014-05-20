#define MAXDAT 65535
#define MAXNOM 255

int conectar();
void inicializar();
void cargar_nombre(char*);
void mostrar_datos(u_int16_t, u_int16_t, u_int16_t, u_int16_t, u_int16_t, char*);
void cargar_mensaje(int, u_int16_t*, u_int16_t*, u_int16_t*, u_int16_t*, u_int16_t*, char*, char*);
void enviar(int, u_int16_t, u_int16_t, u_int16_t, u_int16_t, u_int16_t, char*);
void escuchar(int);
int ultimo_id(char*);

void crear_archivo(int);
void eliminar_archivo(int);
void sincronizar_archivo(int);
void conectar_archivo(int);
void editar_archivo(int, u_int16_t);

void crear_archivo_respuesta(u_int16_t, char*);
void eliminar_archivo_respuesta(u_int16_t, char*);
void sincronizar_archivo_respuesta(u_int16_t, u_int16_t, char*);
void conectar_archivo_respuesta(int, u_int16_t, char*);
void editar_archivo_respuesta(int sock, u_int16_t id);