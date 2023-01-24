#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h> 

#define NULLPROC 0 
#define SUM 1 
#define SUB 2 
#define DIV 3 
#define MULT 4 

typedef 
struct operandos {
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

//Server functions
void 
server_sum (operandos args, int *result);
void 
server_sub (operandos args, int *result);
void 
server_div (operandos args, int *result);
void 
server_mult (operandos args, int *result);

//Client functions
void
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