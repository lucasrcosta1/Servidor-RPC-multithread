#include "svc.h"

void operation_registrar(int * client, Server * server)
{
	int len;
    char xdr_string[MAX_BUFFER];
	size_t size;
	operandos my_obj;
	

    XDR xdr_stream;

	while ((len = read((* client), xdr_string + size, MAX_BUFFER - size - 1)) > 0) {
        size = size + len;
        if (size > MAX_BUFFER - 1 || xdr_string[size - 1] == '\n') {
            break;
        }
    }

	if (len == -1) {
		perror("Não foi possível ler da requisição");
	}

	xdrmem_create(&xdr_stream, xdr_string, len, XDR_ENCODE);

	if (! xdr_my_struct(&xdr_stream, &my_obj)) {
        printf("Error decoding XDR string\n");
        exit(1);
    }

    xdr_destroy(&xdr_stream);
}

void * svc_thread_handler(void * args)
{
    if (args == NULL) return NULL;

	int * client;
	Server * server = (Server *) args;

    while (true) {
        sem_wait(&server->work_avaliable);

        if (first == NULL) {
			sem_post(&server->wait);
			continue;
		}

		client = dequeue();

        sem_post(&server->wait);
        operation_registrar(client, server);
        free(client);
    }

    return NULL;  
}