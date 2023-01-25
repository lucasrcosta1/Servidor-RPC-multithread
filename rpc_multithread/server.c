#include <stdio.h>
#include "rpc.h"
#include "./procedures/sorting/sort.h"
#include "./procedures/sorting/TMat2D.h"

int 
i = 0, 
j = 0;

void 
server_sum (operandos args, int *result) {
	while (i < 100000000) {i++;}
	*result = args.a + args.b;
}

char * 
server_sort () {
	return sort();
}

void 
server_mult_matrix (operandos args, int *result) {
	int r;
	TMat2D *p;
	TMat2D *mat1 = NULL, *mat2 = NULL;
	printf("args.a: %d  | args.b: %d\n",args.a,args.b);
	mat1 = mat2D_create(args.a, args.b);
	mat2 = mat2D_create(args.b, args.a);
	p 	 = mat2D_create(args.a, args.a);

	mat2d_rand_fill(mat1, 1, 5000);
  	mat2d_rand_fill(mat2, 1, 5000);

	if ((r = mat2d_mult (
		mat1, 
		mat2, 
		p
	)) < 0 ) {
		
		printf("Error to calculate r = %d\n",r);
	} else print("Matrix created with success");

	mat2d_free(mat1);
	mat2d_free(mat2);
	mat2d_free(p);
}

void 
server_div (operandos args, int *result) {
	*result = args.a / args.b;
}

