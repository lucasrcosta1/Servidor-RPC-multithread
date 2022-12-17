/*Interface IDL RPC, usada para criacao dos codigos dos stub cliente e servidor
Interface IDL: Camada intermediaria entre cliente e servidor. Quem define o servico ofertado.*/

struct operandos {
/*implementar um atributo char op para que o cliente nem tenha que usar aquele switch no codigo dele*/
    int a;
    int b;
};

program SOMASUB_PROG {
    version SOMASUB_VERSION {
        int SOMA(operandos) = 1;
        int SUB(operandos) = 2;
        int DIVISAO(operandos) = 3;
        int MULT(operandos) = 4;
    } = 1; /*versao procedimento*/
} = 0x20000001; /*versao programa*/
