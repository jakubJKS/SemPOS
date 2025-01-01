CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: server client

server: server.o game_logic.o ipc_utils.o
	$(CC) $(CFLAGS) -o server server.o game_logic.o ipc_utils.o

client: client.o ipc_utils.o
	$(CC) $(CFLAGS) -o client client.o ipc_utils.o

server.o: server.c game_logic.h ipc_utils.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c ipc_utils.h
	$(CC) $(CFLAGS) -c client.c

game_logic.o: game_logic.c game_logic.h
	$(CC) $(CFLAGS) -c game_logic.c

ipc_utils.o: ipc_utils.c ipc_utils.h
	$(CC) $(CFLAGS) -c ipc_utils.c

clean:
	rm -f *.o server client
