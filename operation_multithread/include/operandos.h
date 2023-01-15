#ifndef OPERANDOS_H_DEFINED
#define OPERANDOS_H_DEFINED

typedef struct operandos {
	int a;
	int b;
} Operandos;

typedef struct xdr_message {
	int func;
	int a;
	int b;
} XDRMessage;

typedef struct xdr_operation {
	int func;
	Operandos op;
} XDROp;

#endif