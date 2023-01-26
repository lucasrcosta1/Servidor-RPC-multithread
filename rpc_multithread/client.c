#include "rpc.h"

void *
clear();

int 
main (int argc, char *argv[]) {
	int sock_ret = 0;
	Socket_info *socket_data;

	if (argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}

	sock_ret = create_socket(&socket_data);
	if (sock_ret)
    	process_choice(socket_data);    
	else print("Socket couldn't be created.\n");

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
		case SUM:
            socket_data->operation = choice;
            
			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				print("call failed");
			} else retval = false;

			clear();
			print("Waiting server response");

			retval = recv_operation(&result, socket_data);
			if (!retval) {
				print("call failed");
			} else retval = false;

			printf (
				"Operation: Sum\n\t Returned: %d + %d = %d\n",
				op_arg.a,
				op_arg.b,
				socket_data->result
			);
			break;

		case SORT:
            socket_data->operation = choice;

			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				print("call failed");
			} else retval = false;

			clear();
			print("Waiting server response");

			retval = recv_operation(&result, socket_data);
			if (!retval) {
				print("call failed");
			} else { 
				char *ret_msg;
				if (socket_data->response) ret_msg = "Sort was performed!";
				else ret_msg = "Sort was not performed. Try again later or contact the system Adm.";
				
				printf (
					"Operation: Sort\n\t Return: %s\n",
					ret_msg
				);
				retval = false;
			}

			break;

		case MULT_MATRIX:
            socket_data->operation = choice;

			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				print("call failed");
			} else retval = false;

			clear();
			print("Waiting server response");
			
			retval = recv_operation(&result, socket_data);
			if (!retval) {
				print("call failed");
			} else {
				char *ret_msg;
				if (socket_data->response) ret_msg = "Matrix created with success!";
				else ret_msg = "Matrix was not created. Try again later or contact the system Adm.";
				printf (
					"Operation: Multiply Matriz size: %d x %d\n\t Return: %s" ,
					op_arg.a,
					op_arg.b,
					ret_msg
				);
				retval = false;
			}
			break;

		case DIV:
            socket_data->operation = choice;

			retval = send_operation(&op_arg, socket_data);
			if (!retval) {
				print("call failed");
			} else retval = false;

			clear();
			print("Waiting server response");

			retval = recv_operation(&result, socket_data);
			if (!retval) {
				print("call failed");
			} else retval = false;

			printf (
				"Operation: Div\n\t Returned: %d / %d = %d\n",
				op_arg.a,
				op_arg.b,
				socket_data->result
			);
			break;

		case 5:
			clear();
            print("See you later!\n");
            exit(0);

		default:
			clear();
			print("Command invalid.");
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
    print("\n");
    print("***** Choose an option ******");
    print("(1). Sum two numbers after increment i from 0 to 2 billion");
    print("(2). Sort random 100000 numbers using selection sort ");
    print("(3). Multiply two matrixes");
    print("(4). Divide two numbers after sleep 10 seconds");
    print("(5). Quit");
    print("*****************************");
    
	printf("Type an option => ");
	scanf("%d",&choice);
	if(choice == 1 || choice == 3 || choice == 4) {
		printf("First number => ");
		scanf("%d", &op->a);
		printf("Second number => ");
		scanf("%d", &op->b);
	}

	return choice;
}

/**
 * @brief Clear screen 
 * 
 */
void *
clear() {
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}