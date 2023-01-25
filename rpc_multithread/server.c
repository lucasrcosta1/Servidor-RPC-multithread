#include <stdio.h>
#include "rpc.h"

int 
i = 0, 
j = 0;

void 
server_sum (operandos args, int *result) {
	while (i < 100000000) {i++;}
	*result = args.a + args.b;
}

void 
server_sub (operandos args, int *result) {
	while (j < 100000000) {j++;}
	*result = args.a - args.b;
}

void 
server_mult (operandos args, int *result) {
	*result = args.a * args.b;
}

void 
server_div (operandos args, int *result) {
	*result = args.a / args.b;
}

