#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h> 

#define SOCKET_CREATE_ERR -1
#define NULLPROC 0 
#define SUM 1 
#define PRIME_NUMBERS 2 
#define MULT_MATRIX 3 
#define DIV 4 
#define SOCKET_CREATE_SUCCESS 5
#define MATRIX_MULT_SUCCESS true
#define MATRIX_MULT_ERROR false

typedef 
struct operandos {
	int a, b;
    unsigned long int prime;
} operandos;

typedef 
struct socket_info {
    int socket_created;
    bool response;
	int result;
    int operation;
    int client_struct_length, server_struct_length;
    struct sockaddr_in server_addr, client_addr;
    operandos data;
} Socket_info;

//Server functions
void 
server_sum (operandos args, int *result);
void
server_prime_numbers (unsigned long int value, int *result);
void 
server_div (operandos args, int *result);
bool 
server_mult_matrix (operandos args, int *result);

//Client functions
int
create_socket (Socket_info **socket_data);
void 
process_choice (Socket_info *socket_data);
bool 
send_operation (operandos *argp, Socket_info *socket_data);
bool
recv_operation (int *response, Socket_info *socket_data);
void 
print_socket_info (Socket_info socket_data);

//IO functions
int 
options (operandos *op);
void
print(char *output);