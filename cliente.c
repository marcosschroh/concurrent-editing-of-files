#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXNOM 256 //Máximo tamaño de un nombre de archivo.
#define MAXDAT 65536 //Máximo tamaño de datos que puede ser direccionado con los 2 bytes del header que definen el tamaño.


int main() {
	int s; //File Descriptor del Socket.
	int fin; //Variable utilizada para el ciclo while.
	uint16_t cod; //Código para definir que acción realizar.
	uint16_t tam; //Código para definir el tamaño del mensaje (nombre).
	uint16_t tam2; //Código para definir el tamaño del mensaje (datos).
	char* buffer; //Buffer utilizado para almacenar tanto el nombre del archivo como los datos delimitados por un ";" que luego serán separados.
	char* nombre; //Aca se almacena el nombre del archivo recibido del cliente.
	char* datos; //Aca se almacenan los datos a agregar en el archivo recibidos del cliente.
	struct sockaddr_in server;

	//Inicialización.
	nombre = malloc(MAXNOM);
	memset(nombre, 0, MAXNOM);
	datos = malloc(MAXDAT);
	memset(datos, 0, MAXDAT);

	printf("Iniciando el cliente...\n");

	//Creamos el socket.
	if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	//Seteamos la conexión al servidor.
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(1111);

	//Conectamos con el servidor.
	if (connect(s,(struct sockaddr *)&server,sizeof(server)) == -1) {
		perror("Bind");
		exit(EXIT_FAILURE);
	}

	//While donde elejimos la opción a realizar y enviamos un mensaje al servidor con dicha elección.
	fin = 0;
	while( fin != 6) {
		printf("Elija opción: \n");
		printf("1- Crear archivo.\n2- Borrar archivo.\n3- Solicitar lista.\n4- Enviar datos.\n5- Recibir datos.\n6- Para salir.\n");
		scanf("%d", &fin);
		switch(fin) {
		case 1: //Crear archivo.
			printf("Ingrese nombre del archivo a crear: ");
			scanf("%s", nombre);
			cod = htons(100);
			tam = htons(strlen(nombre));
			buffer = malloc(4 + tam);
			memcpy(buffer, &cod, 2);
			memcpy(buffer + 2, &tam, 2);
			memcpy(buffer + 4, nombre, tam);
			send(s, buffer, 4 + tam, 0);
			break;
		case 2: //Eliminar archivo.
			printf("Ingrese nombre del archivo a borrar: ");
			scanf("%s", nombre);
			cod = htons(200);
			tam = htons(strlen(nombre));
			buffer = malloc(4 + tam);
			memcpy(buffer, &cod, 2);
			memcpy(buffer + 2, &tam, 2);
			memcpy(buffer + 4, nombre, tam);
			send(s, buffer, 4 + tam, 0);
			break;
		case 3: //Solicitar lista de archivos.
			printf("Solicitando lista.\n");
			cod = htons(300);
			buffer = malloc(2);
			memcpy(buffer, &cod, 2);
			send(s, buffer, 2, 0);

			//Aca tengo q empezar a leer datos.

			break;
		case 4: //Agregar datos al archivo.
			printf("Ingrese nombre del archivo a enviar datos: ");
			scanf("%s", nombre);
			printf("Ingrese datos a enviar: ");
			scanf("%s", datos);
			tam = strlen(nombre);
			tam2 = strlen(datos);
			buffer = malloc(4 + tam + 1 + tam2);
			cod = htons(400);
			tam = htons(tam) + htons(tam2) + 1;
			memcpy(buffer, &cod, 2);
			memcpy(buffer + 2, &tam, 2);
			sprintf(buffer + 4, "%s;%s", nombre, datos);
			send(s, buffer, 4 + tam, 0);
			break;
		case 5: //Recibir archivo completo.
			printf("Ingrese nombre del archivo a recibir: ");
			scanf("%s", nombre);
			cod = htons(500);
			tam = htons(strlen(nombre));
			buffer = malloc(4 + tam);
			memcpy(buffer, &cod, 2);
			memcpy(buffer + 2, &tam, 2);
			memcpy(buffer + 4, nombre, tam);
			send(s, buffer, 4 + tam, 0);
			//Aca hay q empezar a leer los datos
			break;
		case 6:
			break;
		default:
			printf("El número ingresado no concuerda con ninguna opción.\n");
			break;
		}
	}
	close(s);
}
