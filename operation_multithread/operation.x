/* 
Pro cenario da aplicacao final, operacoes aplicadas serao:
    - multiplicacao de matrizes (matriz grande);
    - busca de uma string em um arquivo (arquivo grande);
    - encontrar valor primo baseado em um numero grande;
    - busca aplicada utilizando o algoritmo de ordenacao quicksort (grandes arquivos/atentar memoria).
Portanto, a estrutura abaixo provavelmente nao servira o seu proposito. 
*/

struct operandos {
    int a;
    int b;
};

program OPERATION_PROG {
    version OPERATION_VERSION {
        int SOMA(operandos) = 1;
        int SUB(operandos) = 2;
        int DIVISAO(operandos) = 3;
        int MULT(operandos) = 4;
    } = 1; /*versao procedimento*/
} = 0x20000001; /*versao programa*/

