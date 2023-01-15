#include "svc.h"

#define MAX_FUNCS 4

func_t funcs[MAX_FUNCS] = {
    adicao,
    subtracao,
    multiplicacao,
    divisao
};

void operation_registrar(int * client, Server * server)
{
    char buffer[MAX_BUFFER];
    int bytes_received;

    // Receive data from the client
    bytes_received = recv((* client), buffer, MAX_BUFFER, 0);

    if (bytes_received < 0) {
        perror("Error receiving data\n");
        return;
    }

    // Convert the received data to a struct
    XDRMessage * args = (XDRMessage *) buffer;
    XDROp * op = (XDROp *) malloc(sizeof(XDROp));

    if (args->func > MAX_FUNCS) {
        perror("Invalid Function\n");
        return;
    }

    op->func = args->func;
    op->op.a = args->a;
    op->op.b = args->b; 

    // Perform the add operation
    func_t func = funcs[op->func];
    int result = func(op->op);

    // Send the result back to the client
    send(server->socket, &result, sizeof(result), 0);
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