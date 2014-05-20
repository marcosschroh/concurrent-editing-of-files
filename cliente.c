#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "include/cliente.h"

char nombre_archivo_actual[MAXNOM];
char seguir_editando;

int main(int argc, char **argv)
{
	int s; //File Descriptor del Socket.
	int opt; //Variable utilizada para el ciclo
	printf("Iniciando el cliente...\n");

	s = conectar();
	opt = 0;

	while(opt != 7) {
		inicializar();
		printf("Elija opción: \n");
		printf("1- Crear archivo.\n2- Borrar archivo.\n3- Sincronizar archivo.\n4- Editar archivo.\n7- Para salir.\nEleccion: ");
		scanf("%d", &opt);
		printf("\n");
		switch(opt) {
			case 1: crear_archivo(s); break;
			case 2:	eliminar_archivo(s); break;
			case 3: sincronizar_archivo(s); break;
			case 4: conectar_archivo(s); break;
			case 7: printf("Adios!\n"); shutdown(s, 2); return(EXIT_SUCCESS);
			default: printf("El número ingresado no concuerda con ninguna opción.\n"); break;
		}
	}
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

	//Seteamos la conexión al servidor.
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(1111);

	//Conectamos con el servidor.
	if (connect(sock,(struct sockaddr *)&server,sizeof(server)) == -1) {
		perror("Bind");
		exit(EXIT_FAILURE);
	}
	
	return sock;
}

void crear_archivo(int sock){
	char nombre[MAXNOM]; //Aca se almacenan los datos a enviar al servidor.
	u_int16_t longitud; //Indica el tamaño de los datos (variable)

	printf("Ingrese el nombre del archivo a crear: ");
	scanf("%s",nombre);

	cargar_nombre(nombre);
	
	longitud = strlen(nombre);

	enviar(sock, 1, 0, 0, 0, longitud, nombre);
	
	escuchar(sock);
}

void eliminar_archivo(int sock) {
	char nombre[MAXNOM]; //Aca se almacenan los datos a enviar al servidor.
	u_int16_t longitud; //Indica el tamaño de los datos (variable)

	printf("Ingrese el nombre del archivo a eliminar: ");
	scanf("%s",nombre);

	cargar_nombre(nombre);
		
	longitud = strlen(nombre);
	
	enviar(sock, 2, 0, 0, 0, longitud, nombre);
	
	escuchar(sock);
}

void sincronizar_archivo(int sock){
	char nombre[MAXNOM]; //Buffer utilizado para almacenar el mensaje
	u_int16_t longitud; //Indica el tamaño de los datos (variable)

	printf("Ingrese el nombre del archivo a solicitar: ");
	scanf("%s",nombre);
	
	cargar_nombre(nombre);
	
	longitud = strlen(nombre);
		
	enviar(sock, 3, 0, 0, 0, longitud, nombre);
	
	escuchar(sock);
}

void conectar_archivo(int sock){
	char nombre[MAXNOM]; //Aca se almacenan los datos a enviar al servidor.
	u_int16_t longitud; //Indica el tamaño de los datos (variable)

	
	printf("Ingrese el nombre del archivo a conectartse para editar: ");
	scanf("%s",nombre);

	cargar_nombre(nombre);
	
	longitud = strlen(nombre);
	
	enviar(sock, 4, 0, 0, 0, longitud, nombre);
}

int ultimo_id(char* nombre){
	FILE* archivo;
	char* buf;
	int id = 0;
	
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

void cargar_nombre(char* nombre){
	int i;
	
	for(i = 0; i < strlen(nombre); i++){
		nombre_archivo_actual[i] = nombre[i];
	}
}

void inicializar(){
	int i;
	
	for(i = 0; i < MAXNOM; i++){
		nombre_archivo_actual[i] = 0;
	}
}

void enviar(int sock, u_int16_t operacion, u_int16_t id, u_int16_t accion, u_int16_t posicion, u_int16_t longitud, char* datos){
	char* buffer; //Buffer utilizado para almacenar el mensaje

	operacion = htons(operacion);
	id = htons(id);
	accion = htons(accion);
	posicion = htons(posicion);
	longitud = htons(strlen(datos));

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

void escuchar(int sock){
	char* datos; //Aca se almacenan el string.
	char* buffer; //Aca se almacenan los datos recibidos
	u_int16_t operacion; //Indica la operacion a realizar
	u_int16_t id; //Indica el id del archivo.
	u_int16_t accion; //Indica la accion a realizar
	u_int16_t posicion; //Indica la posicion dentro del archivo
	u_int16_t longitud; //Indica el tamaño de los datos (variable)
	
	buffer = malloc(MAXDAT);
	memset(buffer, 0, MAXDAT);
	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);
	
	cargar_mensaje(sock,&operacion,&id,&accion,&posicion,&longitud,datos,buffer);
	mostrar_datos(operacion,id,accion,posicion,longitud,datos);
	switch(operacion){
		case 1:
			crear_archivo_respuesta(id,datos); break;
		case 2:
			eliminar_archivo_respuesta(id,datos); break;
		case 3:
			sincronizar_archivo_respuesta(id, longitud, datos); break;
		case 4:
			conectar_archivo_respuesta(sock, id, datos); break;
		case 5:
			editar_archivo_respuesta(sock, id); break;
		default:
			printf("Error en la recepcion del mensaje.\n"); break;
	}
}

void cargar_mensaje(int sock, u_int16_t* operacion, u_int16_t* id, u_int16_t* accion, u_int16_t* posicion, u_int16_t* longitud, char* datos, char* buffer){
	int cantidad_leida;
	
	cantidad_leida = recv(sock,buffer,10,0);
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
		
	cantidad_leida += recv(sock,datos,(size_t)*longitud,0);
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

void crear_archivo_respuesta(u_int16_t id, char* datos){
	FILE* archivo;
	
	if(id == 1){
		archivo = fopen(nombre_archivo_actual, "w");
		fclose(archivo);
	}
	printf("\n%s\n\n",datos);
}

void eliminar_archivo_respuesta(u_int16_t id, char* datos){
	
	if(id == 1){
		remove(nombre_archivo_actual);
	}
	printf("\n%s\n\n",datos);
}

void sincronizar_archivo_respuesta(u_int16_t id, u_int16_t longitud, char* datos){
	FILE* archivo;
	char* buffer;
	
	buffer = malloc(MAXDAT);
	memset(buffer, 0, MAXDAT);

	if(id == 1) {
		archivo = fopen(nombre_archivo_actual, "w");
		fwrite(datos, sizeof(char), longitud, archivo);
		fclose(archivo);
		printf("Sincronizacion correcta.");
	}
	printf("\n%s\n\n",datos);
}

void conectar_archivo_respuesta(int sock, u_int16_t id, char* datos){
	printf("\n%s\n\n", datos);
	
	if(id != 0){
		editar_archivo(sock, id);
	}	
}

void editar_archivo(int socket, u_int16_t id){
	FILE* archivo;
	char accion;
	char posicion;
	char longitud;
	char* datos;
	char* buffer;
	
	datos = malloc(MAXDAT);
	memset(datos,0,MAXDAT);
	buffer = malloc(MAXDAT);
	memset(buffer,0,MAXDAT);
	
	if(ultimo_id(nombre_archivo_actual) != id){
		enviar(socket, 3, 0, 0, 0, strlen(nombre_archivo_actual), nombre_archivo_actual); //Sincronizamos
		escuchar(socket);
	}
	if((archivo = fopen(nombre_archivo_actual, "r")) != NULL){
		fseek(archivo, 2, SEEK_SET);
		if(!feof(archivo)){
			while (!feof(archivo)){
				fread(buffer, sizeof(char), 1, archivo);
			}
		}
		fclose(archivo);
	}
	printf(buffer); //Mostramos el archivo
	printf("Ingrese accion (1: Agregar, 2: Borrar):");
	scanf(&accion);
	printf("Ingrese posicion:");
	scanf(&posicion);
	if(accion == 1){
		printf("Ingrese datos:");
	} else {
		printf("Ingrese cantidad de caracteres a borrar");
	}
	scanf("%s",datos);	
	longitud = sizeof(datos);
	id++;
	printf("Desea seguir editando este archivo? (1: Si, 0: No):");
	scanf(&seguir_editando);
	enviar(socket, 5, id, accion, posicion, longitud, datos);
	escuchar(socket);
}

void editar_archivo_respuesta(int sock, u_int16_t id){
	
	if(seguir_editando != 1){
		editar_archivo(sock, id);
	}
}
