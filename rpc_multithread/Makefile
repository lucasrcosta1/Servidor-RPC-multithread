client:
	gcc -o client client.c clnt.c -lpthread -lnsl

server: 
	gcc -o server server.c svc.c ./procedures/sorting/TMat2D.c ./procedures/sorting/sort.c -lpthread -lnsl

all:
	gcc -o server server.c svc.c ./procedures/sorting/TMat2D.c ./procedures/sorting/sort.c -lpthread -lnsl

	gcc -o Client client.c clnt.c -lnsl
