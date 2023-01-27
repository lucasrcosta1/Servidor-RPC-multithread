client:
	gcc -o client client.c clnt.c -lpthread -lnsl

server: 
	gcc -o server server.c svc.c ./procedures/matrix/TMat2D.c ./procedures/prime_numbers/prime.c -lpthread -lnsl -lgmp

all:
	gcc -o server server.c svc.c ./procedures/matrix/TMat2D.c ./procedures/prime_numbers/prime.c -lpthread -lnsl -lgmp

	gcc -o Client client.c clnt.c -lnsl 
