#include "TMat2D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct TMat2D {
  int nrows;    // númemro de linhas ;
  int ncolumns; // número de colunas;
  double *data; // dados: local onde armazena os dados da matriz
};

// Função mat2D_create
// Desc - cria uma matriz de i linhas por j colunas que irá conter valores do
// tipo double. Entrada - int nrow: valor inteiro para o número de linhas, int
// ncol: valor inteiro para o número de colunas, Retorno - mat: matriz (TMat2D)
// alocada na heap. Obs: Deverá ser criado um ponteiro para TMat2D e
// inicializado como NULL para receber o retorno da função mat2D_create.
TMat2D *mat2D_create(int nrow, int ncol) {
  TMat2D *mat;
  mat = malloc(sizeof(TMat2D));
  if (mat == NULL) {
    return NULL;
  }
  mat->nrows = nrow;
  mat->ncolumns = ncol;
  mat->data = malloc(ncol * nrow * sizeof(double));

  if (mat->data == NULL) {
    free(mat);
    return NULL;
  }

  return mat;
}

// Função mat2D_rand_fill
// Desc - preenche todos os campos de uma matriz com valores aleatórios no
// intervalo de n1 e n2. Entrada - TMat2D *mat: matriz criada pela função
// mat2D_create(), int n1: valor inteiro que abre o intervalo numérico, int n2:
// valor inteiro que fecha o intervalo numérico, Retorno -
//    0: sucesso
//   -1: erro
int mat2d_rand_fill(TMat2D *mat, int n1, int n2) {
  if (mat == NULL) {
    return -1;
  } else {
    srand(time(NULL));
    int i;
    int n_elements = mat->nrows * mat->ncolumns;
    for (i = 0; i < n_elements; i++) {
      mat->data[i] = (n1) + (rand() % (n2 - n1));
    }
    return 0;
  }
}

// Função mat2D_mult
// Desc - multiplica duas matrizes e armazena o resultado em uma terceira
// matriz. Entrada - TMat2D *mat1: matriz criada pela função mat2D_create(),
// TMat2D *mat2: matriz criada pela função mat2D_create(),
// TMat2D *mult: matriz criada pela função mat2D_create() que armazenará o
// resultado da multiplicação de mat1 e mat2. Obs - a multiplicação ocorrerá
// apenas se: o numero de colunas de mat1 for igual ao numero de linhas de mat2,
// o numero de linhas de mult for igual ao número de linhas de mat1 e o numero
// de colunas de mult for igual ao numero de colunas de mat2, ou seja: (mat1
// i,j) m x p * (mat2 i,j) p x n = (mult i,j) m x n
//    0: sucesso
//   -1: erro
int mat2d_mult(TMat2D *mat1, TMat2D *mat2, TMat2D *mult) {

  if (mat1 == NULL || mat2 == NULL || mult == NULL) {
    return -1;
  } else if (mat1->ncolumns != mat2->nrows) {
    return -2;
  } else if (mult->nrows != mat1->nrows || mult->ncolumns != mat2->ncolumns) {
    return -3;
  } else {

    int row1, row2, col1, col2, i, j, k, pos1, pos2, pos3;
    double temp = 0;

    row1 = mat1->nrows;
    col1 = mat1->ncolumns;
    row2 = mat2->nrows;
    col2 = mat2->ncolumns;

    for (i = 0; i < row1; i++) {
      for (j = 0; j < col2; j++) {
        for (k = 0; k < col1; k++) {
          pos1 = k * mat1->nrows + i;
          pos2 = j * mat2->nrows + k;
          temp += mat1->data[pos1] * mat2->data[pos2];
        }
        pos3 = j * mult->nrows + i;
        mult->data[pos3] = temp;
        temp = 0;
      }
    }
    return 0;
  }
}

void mat2d_print(TMat2D *mat) {
  if (mat == NULL)
    printf("Error");
  else {
    int pos;
    for (int i = 0; i < mat->nrows; i++) {
      for (int j = 0; j < mat->ncolumns; j++) {
        pos = j * mat->nrows + i;
        printf("%.2lf ", mat->data[pos]);
      }
      printf("\n");
    }
  }
}

// return 0: sucesso
// return -1: erro
int mat2d_free(TMat2D *mat) {
  if (mat == NULL) {
    return -1;
  } else {
    free(mat->data);
    free(mat);
    return 0;
  }
}