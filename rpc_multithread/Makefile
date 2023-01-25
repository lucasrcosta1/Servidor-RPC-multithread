client:
	gcc -o client client.c clnt.c -lnsl

server: 
	gcc -o server server.c svc.c -lpthread -lnsl

all:
	gcc -o Server server.c svc.c -lpthread -lnsl
	gcc -o Client client.c clnt.c -lnsl
