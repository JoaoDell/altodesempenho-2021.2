#include "matrix.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

matrixNode *startMatrixNode(double value, uint64_t i, uint64_t j){

    matrixNode *mn = (matrixNode *) malloc(sizeof(matrixNode));
    mn->value = value;
    mn->i = i; mn->j = j;
    return mn;
}

void setMatrixNode(matrixNode *mn, double value, uint64_t i, uint64_t j){
    mn->value = value;
    mn->i = i; mn->j = j;
}

row *startRow(uint64_t rowsize){
    row *r = (row *) malloc(sizeof(row));
    r->nodes = (matrixNode **) malloc(rowsize*sizeof(matrixNode*));
    r->rowsize = rowsize;
    return r;
}



matrix *startMatrix(uint64_t N, uint64_t M){

    matrix *mat = (matrix *) malloc(sizeof(matrix));
    mat->rowlist = (row **) malloc(N*sizeof(row *));
    mat->N = N;
    mat->M = M;
    return mat;
}


matrix *getMatrixFromFile(char *filename){

    uint64_t N, M;


    FILE *raw;
    fopen_s(&raw, filename, "rb");

    char line[500];

    fscanf(raw, "%llu\n", &N);
    fscanf(raw, "%llu\n", &M);
    // printf("%llu\n", N);

    matrix *m = startMatrix(N, M);

    uint64_t iaux = 0, jaux = 0;
    double value;

    uint64_t iaa, jaa;
    double vaa;

    uint64_t rownumbers[N];
    int rowcheck[N];
    for(uint64_t k = 0; k < N; k++){
        rownumbers[k] = 0;
        rowcheck[k] = 0;
    }

    long save_line1 = 0, save_line2 = 0;
    save_line1 = ftell(raw);
    save_line2 = ftell(raw);
    for(uint64_t i = 0; i < M; i++){
        fseek(raw, save_line2, SEEK_SET); //retorna à linha que parou de coletar os índices para comparação
        fscanf(raw, "%llu %llu %lf\n", &iaux, &jaux, &value);
        save_line2 = ftell(raw);
        // printf("%ld\n", save_line2);
        fseek(raw, save_line1, SEEK_SET); //retorna à linha aonde a matriz começa a ser ditada

        for(uint64_t j = 0; j < M; j++){
            fscanf(raw, "%llu %llu %lf\n", &iaa, &jaa, &vaa);
            // printf("%lf, %llu\n", vaa, j);
            // printf("%llu, %llu\n", iaa, iaux);
            if(iaa == iaux && !rowcheck[iaux]) rownumbers[iaux]++;
            // printf("%llu, %llu, %llu\n", iaa, iaux, rownumbers[iaux]);
        }
        rowcheck[iaux] = 1; //checa se aquele elemento daquela linha já foi lido
    }


    for(uint64_t i = 0; i < N; i++){
        m->rowlist[i] = startRow(rownumbers[i]);
        rowcheck[i] = 0;
    }


    fseek(raw, save_line1, SEEK_SET);
    save_line2 = ftell(raw);

    uint64_t counter = 0;
    for(uint64_t i = 0; i < M; i++){
        fseek(raw, save_line2, SEEK_SET); //retorna à linha que parou de coletar os índices para comparação
        fscanf(raw, "%llu %llu %lf\n", &iaux, &jaux, &value);
        save_line2 = ftell(raw);
        // printf("%ld\n", save_line2);
        fseek(raw, save_line1, SEEK_SET); //retorna à linha aonde a matriz começa a ser ditada

        for(uint64_t j = 0; j < M; j++){
            fscanf(raw, "%llu %llu %lf\n", &iaa, &jaa, &vaa);
            // printf("%lf, %llu\n", vaa, j);
            // printf("%llu, %llu\n", iaa, iaux);
            if(iaa == iaux && !rowcheck[iaux]){
                // setMatrixNode(m->rowlist[iaux]->nodes[counter], value, iaux, jaa);
                // printf("%llu\n", counter);
                m->rowlist[iaux]->nodes[counter] = startMatrixNode(vaa, iaux, jaa);
                counter++;
            }
            // printf("%llu, %llu, %lf\n", iaux, iaa, vaa);
        }

        counter = 0;
        rowcheck[iaux] = 1; //checa se aquele elemento daquela linha já foi lido
    }
 
    fclose(raw);


    return m;
}



void printMatrix(matrix m){


    for(uint64_t i = 0; i < m.N; i++){
        char line[500];
        for(uint64_t j = 0; j < m.rowlist[i]->rowsize; j++){
            printf("%lf ", m.rowlist[i]->nodes[j]->value);
            // sprintf(line, "%lf ", m.rowlist[i]->nodes[j]->value);
        }
        sprintf(line, "%s", "\n");
        printf("%s\n", line);
    }

}



double *randArray(uint64_t size){
    srand(0); //Set seed for random generator

    double *array = (double *) malloc(size*sizeof(double));

    for(uint64_t i = 0; i < size; i++){
        array[i] = pow(-1.0, rand())*rand()/RAND_MAX;
    }

    return array;
}

double *zeroArray(uint64_t size){
    double *array = (double *) malloc(size*sizeof(double));
    for(uint64_t i = 0; i < size; i++){
        array[i] = 0;
    }
    return array;
}

void printArray(uint64_t size, double *array){


    printf("[ ");
    for(uint64_t i = 0; i < size - 1; i++){
        printf("%.15lf, ", array[i]);
    }
    printf("%.15lf ]\n", array[size - 1]);
}


void MatArrayMult(matrix m, double *array, double *result){
    for(uint64_t i = 0; i < m.N; i++){
        double r = 0.0;
        for(uint64_t j = 0; j < m.rowlist[i]->rowsize; j++){
            r += m.rowlist[i]->nodes[j]->value*array[m.rowlist[i]->nodes[j]->i];
        }
        result[i] = r;
    }
}


double norm(uint64_t size, double *array){

    double n = 0;

    for(uint64_t i = 0; i < size; i++)
        n += array[i]*array[i];

    return sqrt(n);
}

void normalize(uint64_t size, double *array, double *result){
    for(uint64_t i = 0; i < size; i++)
        result[i] = array[i]/norm(size, array);
}




