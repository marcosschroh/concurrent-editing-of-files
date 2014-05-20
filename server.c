#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



#include "include/server.h"

struct usuarios {
	int usuario;
	char archivo[MAXNOM];
	struct usuarios* siguiente;
};

struct usuarios* primero;
struct usuarios* ultimo;

int main(int argc, char **argv)
{
	int s; //File Descriptor del Socket.
	int sc; //Accept.
	socklen_t n; //Tipo de datos utilizado como último parámetro en la aceptación.
	struct sockaddr_in cliente; //Estructura del Socket cliente.
	char* buffer; //Aca se almacenan los datos recibidos
	char* datos; //Aca se almacenan el string.
	u_int16_t operacion;
	u_int16_t id; //Indica el id del archivo.
	u_int16_t accion;
	u_int16_t posicion; //Indica la posicion dentro del archivo
	u_int16_t longitud; //Indica el tamaño de los datos (variable)
	
	//Inicializacion
	buffer = malloc(MAXDAT);
	memset(buffer, 0, MAXDAT);
	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);
	primero = (struct usuarios*) NULL;
	ultimo = (struct usuarios*) NULL;

	s = conectar();
	n = sizeof(cliente);

	//While donde se aceptan los clientes
	while(1) {
		sc = accept (s, (struct sockaddr *)&cliente, &n);
		printf("Cliente aceptado: %d.\n", sc);
					
		while((cargar_mensaje(sc,&operacion,&id,&accion,&posicion,&longitud,datos,buffer)) > 0){
			mostrar_datos(operacion,id,accion,posicion,longitud,datos);
			switch(operacion) {
				case 1: crear_archivo(sc,datos); break;
				case 2: eliminar_archivo(sc,datos); break;
				case 3: sincronizar_archivo(sc, datos); break;
				case 4: conectar_archivo(sc, datos); break;
				case 5: editar_archivo(sc, id, accion, posicion, longitud, datos); break;
			}
			memset(datos, 0, MAXDAT);
		}
	}
	shutdown(s,2);
	return 0;
}

int conectar(){
	
	struct sockaddr_in server;
	int sock;
	
	//Creamos el socket.
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	
	//Seteamos el servidor.
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(1111);

	//Bindeamos el servidor.
	if((bind(sock, (struct sockaddr *)&server , sizeof(server))) == -1) {
		perror("Bind");
		exit(EXIT_FAILURE);
	}

	//Ponemos a las escucha el servidor un máximo de 5 conexiones concurrentes.
	if((listen (sock,5)) == -1) {
		perror("Listen");
		exit(EXIT_FAILURE);
	}
	
	return sock;
}

void crear_archivo(int sock, char* nombre){
	char* datos;
	FILE* archivo;
	u_int16_t longitud;
	u_int16_t id = 0;
	
	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);

	if((archivo = fopen(nombre, "r")) == NULL){
		archivo = fopen(nombre, "w");
		fclose(archivo);
		id = 1;
		datos = "El archivo fue creado.";
	} else {
		datos = "Ya existe un archivo con ese nombre.";
	}
	longitud = strlen(datos);	
	enviar(sock, 1, id, 0, 0, longitud, datos);
}

void eliminar_archivo(int sock, char* nombre){
	char* datos;
	FILE* archivo;
	u_int16_t longitud;
	u_int16_t id = 0;
	
	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);

	if((archivo = fopen(nombre, "r")) != NULL){
		fclose(archivo);
		remove(nombre);
		datos = "El archivo fue eliminado.";
		id = 1;
	} else {
		datos = "No existe ningun archivo con ese nombre.";
	}	
	longitud = strlen(datos);
	enviar(sock, 2, id, 0, 0, longitud, datos);
}

void sincronizar_archivo(int sock, char* nombre){
	FILE* archivo;
	char* datos;
	u_int16_t longitud;
	u_int16_t id = 0;

	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);
	
	if((archivo = fopen(nombre, "r")) != NULL){
		fseek(archivo, 0, SEEK_END); // Se posiciona al final del archivo
		longitud = ftell(archivo); // Devuelve el tamaño del archivo (en bytes)
		fseek(archivo, 0, SEEK_SET);
		fread(datos,sizeof(char),longitud,archivo);
		fclose(archivo);
		id = 1;

	} else {
		datos = "No existe ningun archivo con ese nombre.";
	}
	longitud = strlen(datos);
	enviar(sock, 3, id, 0, 0, longitud, datos);
}

u_int16_t ultimo_id(char* nombre){
	FILE* archivo;
	char* buf;
	u_int16_t id = 0;
	
	buf = malloc(MAXDAT);
	memset(buf,0,MAXDAT);
	
	if((archivo = fopen(nombre, "r")) != NULL){
		fseek(archivo, 0, SEEK_SET);
		if(!feof(archivo)){
			fread(&id,sizeof(u_int16_t),1,archivo);			
		}
		fclose(archivo);
	}
	return id;
}

void conectar_archivo(int sock, char* nombre){	
	FILE* archivo;
	char* datos;
	u_int16_t longitud;
	u_int16_t id = 0;
	
	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);
	
	if((archivo = fopen(nombre, "r")) != NULL){
		fclose(archivo);
		agregar_usuario(sock, nombre);
		datos = "Se ha conectado al archivo.";
		id = ultimo_id(nombre);
	} else {
		datos = "No existe ningun archivo con ese nombre.";
	}
	longitud = strlen(datos);
	enviar(sock, 4, id, 0, 0, longitud, datos);
}

void enviar(int sock, u_int16_t operacion, u_int16_t id, u_int16_t accion, u_int16_t posicion, u_int16_t longitud, char* datos){
	char* buffer; //Buffer utilizado para almacenar el mensaje

	operacion = htons(operacion);
	id = htons(id);
	accion = htons(accion);
	posicion = htons(posicion);
	longitud = htons(longitud);
	
	buffer = malloc(10 + ntohs(longitud));
	memset(buffer, 0, 10 + ntohs(longitud));
	
	memcpy(buffer, &operacion, 2);
	memcpy(buffer+2, &id, 2);
	memcpy(buffer+4, &accion, 2);
	memcpy(buffer+6, &posicion, 2);
	memcpy(buffer+8, &longitud, 2);
	memcpy(buffer+10, datos, ntohs(longitud));
	
	send(sock, buffer, 10 + ntohs(longitud), 0);	
}

int cargar_mensaje(int sock, u_int16_t* operacion, u_int16_t* id, u_int16_t* accion, u_int16_t* posicion, u_int16_t* longitud, char* datos, char* buffer){
	int cantidad_leida;
	
	while((cantidad_leida = recv(sock,buffer,10,0)) > 0){
		memcpy(operacion, buffer, 2);
		*operacion = ntohs(*operacion);
		memcpy(id, buffer+2, 2);
		*id = ntohs(*id);
		memcpy(accion, buffer+4, 2);
		*accion = ntohs(*accion);
		memcpy(posicion, buffer+6, 2);
		*posicion = ntohs(*posicion);
		memcpy(longitud, buffer+8, 2);
		*longitud = ntohs(*longitud);

		cantidad_leida += recv(sock,datos, (size_t)*longitud,0);
		return cantidad_leida;
	}
	return cantidad_leida;
}

void mostrar_datos(u_int16_t operacion, u_int16_t id, u_int16_t accion, u_int16_t posicion, u_int16_t longitud, char* datos){
	
	printf("Operacion: %d\n", operacion);
	printf("ID: %d\n", id);
	printf("Accion: %d\n", accion);
	printf("Posicion: %d\n", posicion);
	printf("Longitud: %d\n", longitud);
	printf("Datos: %s\n",datos);
	printf("\n\n");
}

void editar_archivo(int sock, u_int16_t id, u_int16_t accion, u_int16_t posicion, u_int16_t longitud, char* datos){
	FILE* archivo;
	char* buffer;
	char* aux;
	int i;
	
	buffer = malloc(MAXDAT);
	memset(buffer, 0, MAXDAT);
	aux = malloc(MAXDAT);
	memset(aux, 0, MAXDAT);
	
	if((archivo = fopen(ver_archivo_usuario(sock), "r")) != NULL){
		fseek(archivo, 2, SEEK_SET);
		if(!feof(archivo)){
			while (!feof(archivo)){
				fread(buffer, sizeof(char), 1, archivo);
			}
		}
		fclose(archivo);
	}
	for(i = 0; i < posicion; i++){
		*(aux+i) = *(buffer+i);
	}
	if(accion==1){
		for(i = 0; i < longitud; i++){
			*(aux+posicion+i) = *(datos+i);
		}
		for(i = 0; i < strlen(buffer) - posicion; i++){
			*(aux+posicion+longitud+i) = *(buffer+posicion+i);
		}
	} else {
		for(i = 0; i < strlen(buffer) - posicion - longitud; i++){
			*(aux+posicion+i) = *(buffer+posicion+longitud+i);
		}
	}
	if((archivo = fopen(ver_archivo_usuario(sock), "w")) != NULL){
		id++;
		fwrite(&id, sizeof(u_int16_t), 1, archivo);
		fwrite(aux, strlen(aux), 1, archivo);
		fclose(archivo);
	}
	enviar(sock, 5, id, 0, 0, 0, NULL); //Suele tirar error el NULL
}

void agregar_usuario(int usuario, char* nombre){
	int i;
	struct usuarios* nuevo;
	nuevo = malloc(sizeof(struct usuarios));
	memset(nuevo, 0, sizeof(struct usuarios));
	
	nuevo->usuario = usuario;
	for(i = 0; i < strlen(nombre); i++){
		nuevo->archivo[i] = nombre[i];
	}
	nuevo->siguiente = NULL;
	
	if (primero == NULL){
		primero = nuevo;
		ultimo = nuevo;
	} else {
		ultimo->siguiente = nuevo;
		ultimo = nuevo;
	}
}

char* ver_archivo_usuario(int usuario){
	struct usuarios* aux;
	aux = primero;
	
	while((aux != NULL) & (aux->usuario != usuario)){
		aux = aux->siguiente;
	}
	if (aux != NULL){
		return aux->archivo;
	}
	return NULL;
}
