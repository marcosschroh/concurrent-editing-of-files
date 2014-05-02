#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#define MAXNOM 256 //Máximo tamaño de un nombre de archivo.
#define MAXDAT 65536 //Máximo tamaño de datos que puede ser direccionado con los 2 bytes del header que definen el tamaño.

// La siguiente estructura es la que se pasa como parámetro a los thread. Se usa una estructura para en un futuro
// poder expandir la cantidad de parametros en un futuro.
struct parametros {
	int fd;
};

void funcion_thread(void *arg); //Función ejecutada por los thread trás su creación.

int main() {

	int s; //File Descriptor del Socket.
	int sc; //Accept.
	socklen_t n; //Tipo de datos utilizado como último parámetro en la aceptación.
	pthread_t tid; //Tipo de datos utilizado para almacenar la información de thread.
	struct sockaddr_in server; //Estructura del Socket servidor.
	struct sockaddr_in cliente; //Estructura del Socket cliente.
	struct parametros* par; //Puntero a la estructura parametros enviada como argumento a la función funcion_thread().

	//Creamos el socket.
	if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	//Seteamos el servidor.
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(1111);

	//Bindeamos el servidor.
	if((bind(s, (struct sockaddr *)&server , sizeof(server))) == -1) {
		perror("Bind");
		exit(EXIT_FAILURE);
	}

	//Ponemos a las escucha el servidor un máximo de 5 conexiones concurrentes.
	if((listen (s,5)) == -1) {
		perror("Listen");
		exit(EXIT_FAILURE);
	}

	n = sizeof(cliente);

	//While donde se aceptan los clientes y se crean los hilos que los atenderán.
	while(1) {
		sc = accept (s, (struct sockaddr *)&cliente, &n);
		par = malloc(sizeof(struct parametros *));
		printf("Cliente aceptado: %d.\n", sc);
		par->fd = sc;
		pthread_create (&tid, NULL, (void *)&funcion_thread, par);
	}
	close(s);
	return 0;
}


void funcion_thread(void *arg) {
	int i; //Un simple contador.
	int fin; //Variable utilizada para el ciclo while.
	char* nombre; //Aca se almacena el nombre del archivo recibido del cliente.
	char* datos; //Aca se almacenan los datos a agregar en el archivo recibidos del cliente.
	char* buffer; //Buffer utilizado para almacenar tanto el nombre del archivo como los datos delimitados por un ";" que luego serán separados.
	char* q; //Puntero que ayuda en la separación de los parámetros nombre y datos.
	char* r; //Puntero que ayuda en la separación de los parámetros nombre y datos.
	struct parametros* pa; //Estructura del tipo parámetros donde se castea los argumentos recibidos de la llamada a la funcion_thread.
	uint16_t cod; //Código para definir que acción realizar.
	uint16_t tam; //Código para definir el tamaño del mensaje.

	//Inicialización.
	nombre = malloc(MAXNOM);
	memset(nombre, 0, MAXNOM);
	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);
	buffer = malloc(MAXDAT);
	memset(buffer, 0, MAXDAT);

	pa = (struct prametros *) arg;

	//Comienzo del ciclo que contiene las acciones por cada código.
	fin = 1;
	while(fin) {
		read(pa->fd, &cod, 2);
		cod = ntohs(cod);
		switch(cod) {
		case 100: //Crear archivo.
			read(pa->fd, &tam, 2);
			tam = ntohs(tam);
			read(pa->fd, nombre, tam);
			printf("Código: %u. Ahora debería crear un archivo con el nombre: %s.\n", cod, nombre);
			break;
		case 200: //Eliminar archivo.
			read(pa->fd, &tam, 2);
			tam = ntohs(tam);
			read(pa->fd, nombre, tam);
			printf("Código: %u. Ahora debería eliminar el archivo con el nombre: %s.\n", cod, nombre);
			break;
		case 300: //Enviar lista de archivos.
			printf("Código: %u. Ahora deberia enviar la lista de archivo al cliente.\n", cod);
				//Aca se tiene q enviar la lista de archivos.
				break;
		case 400: //Agregar datos al archivo.
			read(pa->fd, &tam, 2);
			tam = ntohs(tam);
			read(pa->fd, buffer, tam);
			q = strchr(buffer, ';');
			i = 0;
			r = buffer;
			while(r != q) {
				r++;
				i++;
			}
			strncpy(nombre, buffer, i);
			strncpy(datos, buffer + i + 1, strlen(buffer) - i - 1);
			printf("Código: %u. Ahora deberia agregar al archivo: %s, los datos: %s.\n", cod, nombre, datos);
			break;
		case 500: //Enviar archivo completo.
			read(pa->fd, &tam, 2);
			tam = ntohs(tam);
			read(pa->fd, nombre, tam);
			printf("Código: %u. Ahora deberia enviar los datos del archivo: %s al cliente\n", cod, nombre);
				//Aca tengo q enviar los datos al cliente
			break;
		}
	}
}
