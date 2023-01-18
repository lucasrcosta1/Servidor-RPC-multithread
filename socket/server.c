#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void){
    int socket_created;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length = sizeof(client_addr);
    
	while (1) {
		// Clean buffers:
		memset(server_message, '\0', sizeof(server_message));
		memset(client_message, '\0', sizeof(client_message));
		
		// Create UDP socket:
		socket_created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		if(socket_created < 0){
			printf("Error while creating socket\n");
			return -1;
		}
		printf("Socket created successfully\n");
		// Create UDP socket;
		
		// Set port and IP:
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(3333); //Transforma dados para bytes
		server_addr.sin_addr.s_addr = INADDR_ANY;
		
		// Bind to the set port and IP:
		if(bind(socket_created, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
			printf("Couldn't bind to the port\n");
			return -1;
		}
		printf("Done with binding\n");
		// Bind to the set port and IP;
		
		printf("Listening for incoming messages...\n\n");
		
		// Receive client's message:
		if (recvfrom (
			socket_created, 
			client_message, 
			sizeof(client_message), 
			0,
			(struct sockaddr*) &client_addr, 
			&client_struct_length
		) < 0) {
			printf("Couldn't receive\n");
			return -1;
		}
		printf("Received message from IP: %s and port: %i\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		
		printf("Msg from client: %s\n", client_message);
		// Receive client's message;
	
		// Respond to client:
		strcpy(server_message, client_message);
		
		if (sendto (
			socket_created, 
			server_message, 
			strlen(server_message), 
			0,
			(struct sockaddr*)&client_addr, 
			client_struct_length
		) < 0){
			printf("Can't send\n");
			return -1;
		}
		// Respond to client;
		
		// Close the socket:
		close(socket_created);
	}
    return 0;
}