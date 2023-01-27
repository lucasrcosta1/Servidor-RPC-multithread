typedef struct TMat2D TMat2D;

TMat2D *mat2D_create(int nrow, int ncol);
int mat2d_rand_fill(TMat2D *mat, int n1, int n2);
int mat2d_mult(TMat2D *mat1, TMat2D *mat2, TMat2D *mult);
void mat2d_print(TMat2D *mat);
int mat2d_free(TMat2D *mat);