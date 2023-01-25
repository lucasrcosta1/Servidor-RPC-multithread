
#include "rpc.h"

/**
 * @brief Create a socket object and link address to server
 * 
 * @param socket_data 
 */
void
create_socket (Socket_info **socket_data) {
	*socket_data = malloc(sizeof(Socket_info));
	(*socket_data)->server_struct_length = sizeof((*socket_data)->server_addr);
	
	(*socket_data)->socket_created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create socket:
    if ((*socket_data)->socket_created < 0) {
        printf("Error while creating socket\n");
    } else printf("Socket created successfully\n");
    
    (*socket_data)->server_addr.sin_family = AF_INET;
    (*socket_data)->server_addr.sin_port = htons(9999);
    (*socket_data)->server_addr.sin_addr.s_addr = inet_addr("192.168.99.139");
}

/**
 * @brief Send the required operation to the server so it can be processed
 * 
 * @param argp 
 * @param response 
 * @param socket_data 
 * @return int 
 */
bool 
send_operation (operandos *argp, Socket_info *socket_data) {
    Socket_info send_sock_data = *socket_data;
    send_sock_data.data = *argp;
    
    if (sendto (
        send_sock_data.socket_created, 
        &send_sock_data, //would be probably better if I send only the operators choosen 
		sizeof(send_sock_data),
        0,
        (struct sockaddr*)&send_sock_data.server_addr, 
        (socklen_t)send_sock_data.server_struct_length
    ) < 0) {
        printf("Unable to send message\n");
        return false;
    }
	return true;
	
}

/**
 * @brief Get the required operation processed in the server
 * 
 * @param argp 
 * @param response 
 * @param socket_data 
 * @return int 
 */
bool
recv_operation (int *response, Socket_info *socket_data) {
	int r, x = *response;
    Socket_info recv_sock_data = *socket_data;

	struct sockaddr_in client;
	socklen_t client_size = sizeof(struct sockaddr_in);
	if (recvfrom (
		recv_sock_data.socket_created, 
		&recv_sock_data, 
		sizeof(recv_sock_data), 
		0,
		(struct sockaddr*) &client, 
		&client_size
	) < 0) {
		print("Couldn't receive");
	} else print("Data received");

	*socket_data = recv_sock_data;
	return true;
}


bool
recv_operation_sort (char *file_response, Socket_info *socket_data) {
	int r;
    Socket_info recv_sock_data = *socket_data;

	struct sockaddr_in client;
	socklen_t client_size = sizeof(struct sockaddr_in);
	if (recvfrom (
		recv_sock_data.socket_created, 
		&file_response, 
		sizeof(file_response), 
		0,
		(struct sockaddr*) &client, 
		&client_size
	) < 0) {
		print("Couldn't receive");
	} else print("Data received");

	*socket_data = recv_sock_data;
	printf("%s", file_response);
	return true;
}

 /**
 * @brief Print all socket info
 * 
 * @param socket_data 
 * @returns void
 */
void 
print_socket_info (Socket_info socket_data) {
	printf("\n\n***********print_socket_info()*************\n");
	printf("Socket number: %d\n",socket_data.socket_created);
	printf("client IP: %s\n",inet_ntoa(socket_data.client_addr.sin_addr));
	printf("client port: %i\n",ntohs(socket_data.client_addr.sin_port));
	printf("operation: %d\n", socket_data.operation);
	printf("fst number: %d\n", socket_data.data.a);
	printf("snd number: %d\n", socket_data.data.b);
	printf("server IP: %s\n",inet_ntoa(socket_data.server_addr.sin_addr));
	printf("server port: %i\n",ntohs(socket_data.server_addr.sin_port));
	printf("***********/print_socket_info()*************\n\n");
}

void	
print(char *output) {
	printf("%s\n", output);fflush(stdout);
}