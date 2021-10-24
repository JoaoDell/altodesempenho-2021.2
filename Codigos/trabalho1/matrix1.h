#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


enum Error {SUCESS, BAD_ARGUMENT, NULL_POINTER, NODE_ALREADY_IN_MATRIX, BAD_FILE};


typedef struct matrixNode{

    /*A sparse matrix can be represented by a data structure that stores each non-zero index of it, storing
    its informations on a node, this node, with its i and j indexes, and its value*/

    double value;   //Valor do nó
    uint64_t i, j;  // índices do nó

}matrixNode;


typedef struct matrix{
    /*The matrix struct should have its row size N, total number of elements M, an array storing the size of each row,
    and an array storing each matrix node. */

    uint64_t N, M;
    uint64_t *rowsize;
    matrixNode **nodelist;
} matrix;




matrixNode *startMatrixNode(double value, uint64_t i, uint64_t j);

void setMatrixNode(matrixNode *mn, double value, uint64_t i, uint64_t j);

matrix *startMatrix(uint64_t N,uint64_t M);

void addNode(matrix *m, matrixNode *n);

matrix *getMatrixFromFile(char *filename);

void printMatrix(matrix m);

double *randArray(uint64_t size, int seed);

double *zeroArray(uint64_t size);

void subArray(uint64_t size, double *a, double *b, double *result);

void numbXarray(uint64_t size, double *a, double numb, double *result);

void printArray(uint64_t size, double *array);

void MatArrayMult(matrix m, double *array, double *result);

double norm(uint64_t size, double *array);

void normalize(uint64_t size, double *array, double *result);

int writeToFile(char *filename, double eigenvalue, double *eigenvector, uint64_t matrix_size);
