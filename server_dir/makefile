CC = gcc

CFLAGS = -c

OBJS = server.o protocol.o file_manager.o

server : ${OBJS}
	${CC} -o server -lpthread ${OBJS}

server.o : server.c include/server.h include/protocol.h include/file_manager.h
	${CC} ${CFLAGS} server.c

protocol.o: protocol.c include/protocol.h
	${CC} ${CFLAGS} protocol.c

file_manager.o: file_manager.c include/protocol.h include/file_manager.h
	${CC} ${CFLAGS} file_manager.c

clean:
	rm -f server server.o protocol.o file_manager.o
