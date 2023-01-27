#include <stdio.h>
#include "rpc.h"
#include "./procedures/sorting/sort.h"
#include "./procedures/sorting/TMat2D.h"

/**
 * @brief Increment a counter from 0 to 2 billion and sum two values given by the client
 * 
 * @param args 
 * @param result 
 */
void 
server_sum (operandos args, int *result) {
	int i =  0;
	while (i < 2000000000) {i++;}
	*result = args.a + args.b;
}

char * 
server_sort () {
	return sort();
}

bool 
server_mult_matrix (operandos args, int *result) {
	int r;
	TMat2D *p;
	TMat2D *mat1 = NULL, *mat2 = NULL;
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
		mat2d_free(mat1);
		mat2d_free(mat2);
		mat2d_free(p);

		return MATRIX_MULT_ERROR;
	} else {
		mat2d_free(mat1);
		mat2d_free(mat2);
		mat2d_free(p);
		return MATRIX_MULT_SUCCESS;
	}

}

void 
server_div (operandos args, int *result) {
	sleep(10);
	*result = args.a / args.b;
}

