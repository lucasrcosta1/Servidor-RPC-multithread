client:
	gcc -o Client operation_client.c operation_clnt.c operation_xdr.c -lnsl

server: 
	gcc -o Server operation_server.c operation_svc.c operation_xdr.c -lpthread -lnsl

all:
	gcc -o Server operation_server.c operation_svc.c operation_xdr.c -lpthread -lnsl
	gcc -o Client operation_client.c operation_clnt.c operation_xdr.c -lnsl