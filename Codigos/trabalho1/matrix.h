#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


enum Error {SUCESS, BAD_ARGUMENT, BAD_ALLOCATION, NODE_ALREADY_IN_MATRIX, BAD_FILE};


typedef struct matrixNode{

    /*A sparse matrix can be represented by a data structure that stores each non-zero index of it, storing
    its informations on a node, this node, with its i and j indexes, and its value*/

    double value;   //Valor do nó
    uint32_t i, j;  // índices do nó

}matrixNode;


typedef struct matrix{
    /*The matrix struct should have its row size N, total number of elements M, an array storing the size of each row,
    and an array storing each matrix node. */

    /*Although its representation its different from a traditional matrix, the row-major representation is still
    used to avoid loss of performance due to much thrashing*/

    uint32_t N, M;
    uint32_t *rowsize;
    matrixNode **nodelist;
} matrix;




matrixNode *startMatrixNode(double value, uint32_t i, uint32_t j);

void setMatrixNode(matrixNode *mn, double value, uint32_t i, uint32_t j);

matrix *startMatrix(uint32_t N,uint32_t M);

void addNode(matrix *m, matrixNode *n);

matrix *getMatrixFromFile(char *filename);

void printMatrix(matrix m);

double *randArray(uint32_t size, int seed);

double *zeroArray(uint32_t size);

void subArray(uint32_t size, double *a, double *b, double *result);

void numbXarray(uint32_t size, double *a, double numb, double *result);

void copyArray(uint32_t size, double *array, double *result);

void printArray(uint32_t size, double *array);

void MatArrayMult(matrix m, double *array, double *result);

double norm(uint32_t size, double *array);

void normalize(uint32_t size, double *array, double *result);

int writeToFile(char *filename, double eigenvalue, double *eigenvector, uint32_t matrix_size);

void checkSign(matrix *m, double *array, double *eigenvalue);
