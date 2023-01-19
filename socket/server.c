#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h> // move it to the .h

pthread_t p_thread;
pthread_attr_t attr;

typedef 
struct socket_info {
    int socket_created;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length;
} Socket_info;

void 
svc_run (Socket_info *socket_data);
void
*thread_context (void *socket_data);
void 
print_socket_info (Socket_info socket_data);

int 
main(void){
	Socket_info socket_data;
	socket_data.client_struct_length = sizeof(socket_data.client_addr);
    
	while (1) {
		// Clean buffers:
		memset(socket_data.server_message, '\0', sizeof(socket_data.server_message));
		memset(socket_data.client_message, '\0', sizeof(socket_data.client_message));
		
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
		
		// Bind to the set port and IP:
		if(bind(socket_data.socket_created, (struct sockaddr*)&socket_data.server_addr, sizeof(socket_data.server_addr)) < 0){
			printf("Couldn't bind to the port\n");
			return -1;
		}
		printf("Done with binding\n");
		// Bind to the set port and IP;
		
		svc_run(&socket_data);

		close(socket_data.socket_created);
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
	Socket_info *auxiliary_sock_data = (Socket_info *)malloc(sizeof(Socket_info));
	*auxiliary_sock_data = *socket_data;

	Socket_info test = *socket_data;
	printf("sizeof auxiliary_sock_data: %lu\n", sizeof(auxiliary_sock_data));
	printf("sizeof test: %lu\n", sizeof(test));

	printf("Listening for incoming messages...\n\n");
		
	// Receive client's message:
	if (recvfrom (
		auxiliary_sock_data->socket_created, 
		auxiliary_sock_data->client_message, 
		sizeof(auxiliary_sock_data->client_message), 
		0,
		(struct sockaddr*) &auxiliary_sock_data->client_addr, 
		&auxiliary_sock_data->client_struct_length
	) < 0) {
		printf("Couldn't receive\n");
	}
	printf (
		"Received message from IP: %s and port: %i\n",
		inet_ntoa(auxiliary_sock_data->client_addr.sin_addr), 
		ntohs(auxiliary_sock_data->client_addr.sin_port)
	);
		
	strcpy(auxiliary_sock_data->server_message, auxiliary_sock_data->client_message);

	// Receive client's message;
	printf("Msg from client: %s\n",auxiliary_sock_data->client_message);

	s = pthread_create(&p_thread,&attr,thread_context,(void *)auxiliary_sock_data);
}

/**
 * @brief Deal with the connection's response in a thread context
 * 
 * @param socket_data 
 * @return void*
 */
void *thread_context (void *socket_data) {
	Socket_info auxiliary_sock_data = *(Socket_info *)socket_data;
  	print_socket_info(auxiliary_sock_data);

	if (sendto (
		auxiliary_sock_data.socket_created, 
		auxiliary_sock_data.server_message, 
		strlen(auxiliary_sock_data.server_message), 
		0,
		(struct sockaddr*)&auxiliary_sock_data.client_addr, 
		auxiliary_sock_data.client_struct_length
	) < 0) {
		printf("Can't send\n");
	}
	// Respond to client:
	printf("server message after copy: %s\n", auxiliary_sock_data.server_message);
}

/**
 * @brief Print all socket infor
 * 
 * @param socket_data 
 * @returns void
 */
void 
print_socket_info (Socket_info socket_data) {
	printf("\n\n***********print_socket_info()*************\n");
	printf("client IP: %s\n",inet_ntoa(socket_data.client_addr.sin_addr));
	printf("client port: %i\n",ntohs(socket_data.client_addr.sin_port));
	printf("Msg from client: %s\n", socket_data.client_message);
	printf("server IP: %s\n",inet_ntoa(socket_data.server_addr.sin_addr));
	printf("server port: %i\n",ntohs(socket_data.server_addr.sin_port));
	printf("Msg from server: %s\n", socket_data.server_message);
	printf("***********/print_socket_info()*************\n\n");
}