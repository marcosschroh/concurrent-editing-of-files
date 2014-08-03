CC = gcc

CFLAGS = -c

OBJS = cliente.o protocol.o file_manager.o

cliente : ${OBJS}
	${CC} -o cliente ${OBJS}

cliente.o : cliente.c include/cliente.h include/protocol.h include/file_manager.h
	${CC} ${CFLAGS} cliente.c

protocol.o: protocol.c include/protocol.h
	${CC} ${CFLAGS} protocol.c

file_manager.o: file_manager.c include/protocol.h include/file_manager.h
	${CC} ${CFLAGS} file_manager.c

clean:
	rm -f cliente cliente.o protocol.o file_manager.o
