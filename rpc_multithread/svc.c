#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpc.h"


pthread_t p_thread;
pthread_attr_t attr;

void 
svc_run (Socket_info *socket_data);
void 
svc_sendreply (Socket_info *socket_data);
void
*thread_context (void *socket_data);
void 
print_socket_info (Socket_info socket_data);

int 
main(void){
	int yes=1;
	Socket_info socket_data;
	socket_data.client_struct_length = sizeof(socket_data.client_addr);
    
	while (1) {
		socket_data.data.a = 0;
		socket_data.data.b = 0;
		
		socket_data.socket_created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		if(socket_data.socket_created < 0){
			print("Error while creating socket");
			return -1;
		}
		
		socket_data.server_addr.sin_family = AF_INET;
		socket_data.server_addr.sin_port = htons(9999); //Transforma dados para bytes
		socket_data.server_addr.sin_addr.s_addr = INADDR_ANY;
		
		if (setsockopt(socket_data.socket_created, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
			perror("setsockopt");
			exit(1);
		} // Reuse socket

		if (bind (
			socket_data.socket_created, 
			(struct sockaddr*)&socket_data.server_addr, 
			sizeof(socket_data.server_addr)
		) < 0){
			print("Couldn't bind to the port");
			return -1;
		}
		
		svc_run(&socket_data);
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
	Socket_info *thread_socket = (Socket_info *)malloc(sizeof(Socket_info));
	Socket_info auxiliary_sock_data = *socket_data;
	
	print("Listening for incoming messages...");

	int data;
	struct sockaddr_in client;
	socklen_t client_size = sizeof(struct sockaddr_in);
	if (recvfrom (
		auxiliary_sock_data.socket_created, 
		&auxiliary_sock_data, 
		sizeof(Socket_info), 
		0,
		(struct sockaddr*) &client, 
		&client_size
	) < 0) {
		print("Couldn't receive");
	}

	auxiliary_sock_data.client_addr = client;
	auxiliary_sock_data.client_struct_length = client_size;

	printf (
		"\nReceived message from IP: %s and port: %i - Socket: %d\n",
		inet_ntoa(auxiliary_sock_data.client_addr.sin_addr), 
		ntohs(auxiliary_sock_data.client_addr.sin_port),
		auxiliary_sock_data.socket_created
	);

	*thread_socket = auxiliary_sock_data;
	pthread_create(&p_thread,&attr,thread_context,(void *)thread_socket);
}

/**
 * @brief Deal with the connection's response in a thread context
 * 
 * @param socket_data 
 * @return void*
 */
void 
*thread_context (void *socket_data) {
	bool response = false;
	Socket_info auxiliary_sock_data = *(Socket_info *)socket_data;
	auxiliary_sock_data.client_struct_length = sizeof(auxiliary_sock_data.client_addr);
	
	switch (auxiliary_sock_data.operation) {
	case NULLPROC:
		svc_sendreply(&auxiliary_sock_data);

	case SUM:
		server_sum(auxiliary_sock_data.data,&auxiliary_sock_data.result);
		svc_sendreply(&auxiliary_sock_data);
		// close(auxiliary_sock_data.socket_created);
		break;

	case PRIME_NUMBERS:
		server_prime_numbers(auxiliary_sock_data.data.prime,&auxiliary_sock_data.result);
		if (auxiliary_sock_data.result == 1)
			auxiliary_sock_data.response = true;
		else auxiliary_sock_data.response = false;
		svc_sendreply(&auxiliary_sock_data);
		break;

	case MULT_MATRIX:
		response = server_mult_matrix(auxiliary_sock_data.data,&auxiliary_sock_data.result);
		auxiliary_sock_data.response = response;
		svc_sendreply(&auxiliary_sock_data);
		break;

	case DIV:
		server_div(auxiliary_sock_data.data,&auxiliary_sock_data.result);
		svc_sendreply(&auxiliary_sock_data);
		break;


	default:
		print("Option not recognized.");
		exit(1);
	}

	pthread_exit(NULL);
}

/**
 * @brief Return value to client after calculated
 * 
 * @param socket_data 
 */
void 
svc_sendreply (Socket_info *socket_data) {
	int r;
	char s[255];

	Socket_info auxiliary_sock_data = *socket_data;
	struct sockaddr_in client = socket_data->client_addr;
	socklen_t client_size = sizeof(struct sockaddr_in);
	if ((r = sendto (
		auxiliary_sock_data.socket_created, 
		&auxiliary_sock_data, 
		sizeof(auxiliary_sock_data), 
		0,
		(struct sockaddr*)&client, 
		(socklen_t)client_size
	))  < 0) {
		print_socket_info(auxiliary_sock_data);
		print("Can't send");
		perror(s);
		// printf("%s\n",s);
	} else printf("\nData sent to client IP: %s and port: %i - Socket: %d\n",
		inet_ntoa(auxiliary_sock_data.client_addr.sin_addr), 
		ntohs(auxiliary_sock_data.client_addr.sin_port),
		auxiliary_sock_data.socket_created
	);
	
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

/**
 * @brief Simplify the printf and fflush sintax to display a message  
 * 
 * @param output 
 */
void
print(char *output) {
	printf("%s\n", output);fflush(stdout);
}
