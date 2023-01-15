COMPILER = gcc

FLAGS  = -lnsl -ltirpc -lpthread -Wall -fexceptions -g -Iinclude -I/usr/include/tirpc
OBJ = obj
SRC = src

CLIENT_FOLDER = src/client
SERVER_FOLDER = src/server

C_FILENAMES = queue.c # operation_xdr.c
C_CLIENT_FILENAMES = operation_client.c operation_clnt.c
C_SERVER_FILENAMES = main.c server.c svc.c functions.c # operation.c

C_FULL_PATH = $(patsubst %.c, $(SRC)/%.c, $(C_FILENAMES) )
C_FULL_PATH_CLIENT = $(patsubst %.c, $(CLIENT_FOLDER)/%.c, $(C_CLIENT_FILENAMES) )
C_FULL_PATH_SERVER = $(patsubst %.c, $(SERVER_FOLDER)/%.c, $(C_SERVER_FILENAMES) )


client:
	gcc -o Client $(C_FULL_PATH_CLIENT) $(C_FULL_PATH) $(FLAGS)

server: 
	gcc -o Server $(C_FULL_PATH_SERVER) $(C_FULL_PATH) $(FLAGS)

all: server client