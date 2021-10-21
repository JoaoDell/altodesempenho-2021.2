#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


enum Error {SUCESS, BAD_ARGUMENT, NULL_POINTER, NODE_ALREADY_IN_MATRIX};


typedef struct matrixNode{
    /*Uma matrix esparsa pode ser representada por uma estrutura de dados que guarda cada índice não-nulo da matriz,
    armazenando as suas informações em um nó com uma casa de valor, os índices i e j*/

    double value;   //Valor do nó
    uint64_t i, j;  // índices do nó
    // struct matrixNode *right; //Ponteiros para o próximo nó à direita e abaixo

}matrixNode;

typedef struct row{
    matrixNode **nodes;
    uint64_t rowsize;
} row;



typedef struct matrix{
    uint64_t N, M;
    row **rowlist;
} matrix;




matrixNode *startMatrixNode(double value, uint64_t i, uint64_t j);

void setMatrixNode(matrixNode *mn, double value, uint64_t i, uint64_t j);

row *startRow(uint64_t rowsize);

matrix *startMatrix(uint64_t N,uint64_t M);

void addNode(matrix *m, matrixNode *n);

matrix *getMatrixFromFile(char *filename);

void printMatrix(matrix m);

double *randArray(uint64_t size);

double *zeroArray(uint64_t size);

void printArray(uint64_t size, double *array);

void MatArrayMult(matrix m, double *array, double *result);

double norm(uint64_t size, double *array);

void normalize(uint64_t size, double *array, double *result);
