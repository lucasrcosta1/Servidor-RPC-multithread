#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

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

int 
options (operandos *op);
void 
process_choice (Socket_info *socket_data);
bool 
send_operation (operandos *argp, Socket_info *socket_data);
bool
recv_operation (int *response, Socket_info *socket_data);
void 
print_socket_info (Socket_info socket_data);

int 
main (int argc, char *argv[]) {
	Socket_info *socket_data = (Socket_info *)malloc(sizeof(Socket_info));
	socket_data->server_struct_length = sizeof(socket_data->server_addr);

	if (argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
   
    socket_data->socket_created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create socket:
    if (socket_data->socket_created < 0) {
        printf("Error while creating socket\n");
        return -1;
    } else printf("Socket created successfully\n");
    
    socket_data->server_addr.sin_family = AF_INET;
    socket_data->server_addr.sin_port = htons(3333);
    socket_data->server_addr.sin_addr.s_addr = inet_addr("192.168.99.139");
    
    socket_data->client_addr.sin_family = AF_INET;
    socket_data->client_addr.sin_port = htons(4444);
	// socket_data->client_addr.sin_addr.s_addr = INADDR_ANY;    
	socket_data->client_addr.sin_addr.s_addr = inet_addr("192.168.99.140");  
    process_choice(socket_data);    
    
    printf("Server's response: %d\n", socket_data->operation);
    
    // Close the socket:
    close(socket_data->socket_created);
    
    return 0;
}

/**
 * @brief Provide a sequence of operations and its respective processing
 * 
 * @param socket_data 
 */
void 
process_choice (Socket_info *socket_data) {
	bool retval = false;
	int result, choice;
	operandos op_arg;

    while (1) {
		choice = options(&op_arg);
		switch (choice) {
		case 1:
            socket_data->operation = choice;
			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;
			printf("pass here?");fflush(stdout);
			retval = recv_operation(&result, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;

			printf("%d + %d = %d",op_arg.a,op_arg.b,result);
			break;

		case 2:
            socket_data->operation = choice;
			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;
			printf("pass here?");fflush(stdout);
			retval = recv_operation(&result, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;

			printf("%d + %d = %d",op_arg.a,op_arg.b,result);
			break;

		case 3:
            socket_data->operation = choice;
			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;
			printf("pass here?");fflush(stdout);
			retval = recv_operation(&result, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;

			printf("%d + %d = %d",op_arg.a,op_arg.b,result);
			break;

		case 4:
            socket_data->operation = choice;
			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;
			printf("pass here?");fflush(stdout);
			retval = recv_operation(&result, socket_data);
			if (!retval) {
				printf("call failed");
			} else retval = false;

			printf("%d + %d = %d",op_arg.a,op_arg.b,result);
			break;

		case 5:
            printf("See you later!\n");
            exit(0);

		default:
			printf("Command invalid.");
			break;
		}
	}
}

/**
 * @brief Display all the operations that the user has to choose
 * 
 * @param op 
 * @return int 
 */
int 
options (operandos *op) {
	int choice;
    printf("\n\n");
    printf("***** Choose an option ******\n");
    printf("(1). Sum two numbers\n");
    printf("(2). Subtract two numbers \n");
    printf("(3). Divide two numbers\n");
    printf("(4). Multiply two numbers\n");
    printf("(5). Quit\n");
    printf("*****************************\n");
    
	printf("Type an option => \t");
	scanf("%d",&choice);
	if(choice < 5) {
		printf("\nFirst number =>\t");
		scanf("%d", &op->a);
		printf("Second number =>\t");
		scanf("%d", &op->b);
	}

	return choice;
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
    
	printf("Before sendto to be called\n");fflush(stdout);
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
	printf("After sendto to be called\n"); fflush(stdout);
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
	int r;
    Socket_info recv_sock_data = *socket_data;

    // print_socket_info(auxiliary_sock_data);
    // Receive the server's response:
	printf("Before recvfrom be called");fflush(stdout);
	print_socket_info(recv_sock_data);
    if ((r = recvfrom (
        recv_sock_data.socket_created, 
        &recv_sock_data.result,
        sizeof(recv_sock_data.result), 
        0, 
        (struct sockaddr*)&recv_sock_data.server_addr, 
        &recv_sock_data.server_struct_length
    )) < 0) {
        printf("Error while receiving server's msg\n");
        return false;
    }
	// r = read(recv_sock_data.socket_created, 
    //     &recv_sock_data.result,
    //     1);
	printf("After recvfrom be called, r = %d",r);fflush(stdout);
	
	printf("server result after operation: %d r = %d\n", recv_sock_data.result, r);fflush(stdout);
    print_socket_info(recv_sock_data);

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