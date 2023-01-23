#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h> // move it to the .h

#define NULLPROC 0 // move it to the .h
#define SUM 1 // move it to the .h
#define SUB 2 // move it to the .h
#define DIV 3 // move it to the .h
#define MULT 4 // move it to the .h

pthread_t p_thread;
pthread_attr_t attr;

typedef struct operandos {
	int a, b;
} operandos;

typedef 
struct socket_info {
    int socket_created;
	int result;
    int operation;
    int client_struct_length, server_struct_length;
    struct sockaddr_in server_addr, client_addr;
    operandos data;
} Socket_info;

void 
svc_run (Socket_info *socket_data);
void 
svc_sendreply (Socket_info socket_data);
void
*thread_context (void *socket_data);
void 
server_sum (operandos args, int *result);
void 
server_sub (operandos args, int *result);
void 
server_div (operandos args, int *result);
void 
server_mult (operandos args, int *result);
void 
print_socket_info (Socket_info socket_data);

int 
main(void){
	Socket_info socket_data;
	socket_data.client_struct_length = sizeof(socket_data.client_addr);
    
	while (1) {
		// Clean buffers:
		socket_data.data.a = 0;
		socket_data.data.b = 0;
		
		// Create UDP socket:
		socket_data.socket_created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		if(socket_data.socket_created < 0){
			printf("Error while creating socket\n");
			return -1;
		}
		printf("Socket created successfully\n");
		// Create UDP socket;
		
		// Set port and IP:
		socket_data.server_addr.sin_family = AF_INET;
		socket_data.server_addr.sin_port = htons(3333); //Transforma dados para bytes
		socket_data.server_addr.sin_addr.s_addr = INADDR_ANY;
		
		if (bind (
			socket_data.socket_created, 
			(struct sockaddr*)&socket_data.server_addr, 
			sizeof(socket_data.server_addr)
		) < 0){
			printf("Couldn't bind to the port\n");
			return -1;
		}
		printf("Done with binding\n");
		
		svc_run(&socket_data);


		printf("After svc_run\n");
	}
    return 0;
}

/**
 * @brief Listen to a socket, receive and respond its connections
 * 
 * @param socket_data 
 */
void 
svc_run (Socket_info *socket_data) {
	int s;
	caddr_t data_to_receive, result;

	Socket_info *thread_socket = (Socket_info *)malloc(sizeof(Socket_info));
	Socket_info auxiliary_sock_data = *socket_data;

	printf("Listening for incoming messages...\n");

	// Receive client's message:
	if (recvfrom (
		auxiliary_sock_data.socket_created, 
		&auxiliary_sock_data, 
		sizeof(auxiliary_sock_data), 
		0,
		(struct sockaddr*) &auxiliary_sock_data.client_addr, 
		&auxiliary_sock_data.client_struct_length
	) < 0) {
		printf("Couldn't receive\n");
	}

	printf (
		"Received message from IP: %s and port: %i\n",
		inet_ntoa(auxiliary_sock_data.client_addr.sin_addr), 
		ntohs(auxiliary_sock_data.client_addr.sin_port)
	);
		
	// print_socket_info(auxiliary_sock_data);

	*thread_socket = auxiliary_sock_data;
	printf("antes criacao thread\n");fflush(stdout);
	s = pthread_create(&p_thread,&attr,thread_context,(void *)thread_socket);
	printf("thread value s = %d\n",s);fflush(stdout);
	getchar();
}

/**
 * @brief Deal with the connection's response in a thread context
 * 
 * @param socket_data 
 * @return void*
 */
void 
*thread_context (void *socket_data) {
	Socket_info auxiliary_sock_data = *(Socket_info *)socket_data;
	auxiliary_sock_data.client_struct_length = sizeof(auxiliary_sock_data.client_addr);
  	print_socket_info(auxiliary_sock_data);
	printf("antes do switch\n"); fflush(stdout);
	switch (auxiliary_sock_data.operation) {
	case NULLPROC:
		svc_sendreply(auxiliary_sock_data);

	case SUM:
		printf("dentro do case SUM\n"); fflush(stdout);
		server_sum(auxiliary_sock_data.data,&auxiliary_sock_data.result);
		svc_sendreply(auxiliary_sock_data);
		printf("depois svc_sendreply\n"); fflush(stdout);
		getchar();
		close(auxiliary_sock_data.socket_created);
		break;

	case SUB:
		server_sub(auxiliary_sock_data.data,&auxiliary_sock_data.result);
		svc_sendreply(auxiliary_sock_data);
		close(auxiliary_sock_data.socket_created);
		break;

	case DIV:
		server_div(auxiliary_sock_data.data,&auxiliary_sock_data.result);
		svc_sendreply(auxiliary_sock_data);
		close(auxiliary_sock_data.socket_created);
		break;

	case MULT:
		server_mult(auxiliary_sock_data.data,&auxiliary_sock_data.result);
		svc_sendreply(auxiliary_sock_data);
		close(auxiliary_sock_data.socket_created);
		break;

	default:
		printf("Option not recognized.\n");
		exit(1);
	}
	printf("terminando a thread\n");fflush(stdout);
	pthread_exit(NULL);
}

void 
svc_sendreply (Socket_info socket_data) {
	int r;
	Socket_info auxiliary_sock_data = socket_data;
	printf("antes sendto\n");fflush(stdout);
	if ((r = sendto (
		auxiliary_sock_data.socket_created, 
		&auxiliary_sock_data.result, 
		sizeof(auxiliary_sock_data.result), 
		0,
		(struct sockaddr*)&auxiliary_sock_data.client_addr, 
		(socklen_t)auxiliary_sock_data.client_struct_length
	))  < 0) {
		printf("Can't send\n");
	}
	printf("depois sendto\n");fflush(stdout);
	// Respond to client:
	printf("server result after operation: %d r = %d\n", auxiliary_sock_data.result, r);fflush(stdout);
  	// print_socket_info(auxiliary_sock_data);
}

void 
server_sum (operandos args, int *result) {
	*result = args.a + args.b;
}

void 
server_sub (operandos args, int *result) {
	*result = args.a - args.b;
}

void 
server_div (operandos args, int *result) {
	*result = args.a * args.b;
}

void 
server_mult (operandos args, int *result) {
	*result = args.a / args.b;
}


 /**
 * @brief Print all socket info
 * 
 * @param socket_data 
 * @returns void
 */
void 
print_socket_info (Socket_info socket_data) {
	printf("\n\n***********print_socket_info()*************\n"); fflush(stdout);
	printf("Socket number: %d\n",socket_data.socket_created); fflush(stdout);
	printf("client IP: %s\n",inet_ntoa(socket_data.client_addr.sin_addr)); fflush(stdout);
	printf("client port: %i\n",ntohs(socket_data.client_addr.sin_port)); fflush(stdout);
	printf("operation: %d\n", socket_data.operation); fflush(stdout);
	printf("fst number: %d\n", socket_data.data.a); fflush(stdout);
	printf("snd number: %d\n", socket_data.data.b); fflush(stdout);
	printf("Result: %d\n", socket_data.result); fflush(stdout);
	printf("server IP: %s\n",inet_ntoa(socket_data.server_addr.sin_addr)); fflush(stdout);
	printf("server port: %i\n",ntohs(socket_data.server_addr.sin_port)); fflush(stdout);
	printf("***********/print_socket_info()*************\n\n"); fflush(stdout);
}

