#include "matrix.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

matrixNode *startMatrixNode(double value, uint64_t i, uint64_t j){

    matrixNode *mn = (matrixNode *) malloc(sizeof(matrixNode));
    if(mn == NULL) {
        fprintf(stderr, "%s", "Matrix Node could not be allocated!\n");
        return NULL;
    }
    else{
        mn->value = value;
        mn->i = i; mn->j = j;
        return mn;
    }
}

void setMatrixNode(matrixNode *mn, double value, uint64_t i, uint64_t j){
    mn->value = value;
    mn->i = i; mn->j = j;
}


matrix *startMatrix(uint64_t N, uint64_t M){

    matrix *mat = (matrix *) malloc(sizeof(matrix));
    if(mat == NULL) {
        fprintf(stderr, "%s", "Matrix could not be allocated!\n");
        return NULL;
    }
    else{
        mat->rowsize = (uint64_t *) malloc(N*sizeof(uint64_t));
        mat->nodelist = (matrixNode **) malloc(M*sizeof(matrixNode *));
        if(mat->rowsize == NULL || mat->nodelist == NULL){
            fprintf(stderr, "%s", "Matrix elements could not be allocated!\n");
            return NULL;
        }
        else{
            mat->N = N;
            mat->M = M;
            return mat;
        }
    }
}


matrix *getMatrixFromFile(char *filename){

    uint64_t N, M;


    FILE *raw;
    fopen_s(&raw, filename, "rb");

    char line[500];

    //Gets N and M from file
    fscanf(raw, "%llu\n", &N);
    fscanf(raw, "%llu\n", &M);

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

    /*FIRST PART - Collecting the size of the rows*/
    long save_line1 = 0, save_line2 = 0;
    save_line1 = ftell(raw);
    save_line2 = ftell(raw);
    for(uint64_t i = 0; i < M; i++){
        fseek(raw, save_line2, SEEK_SET); //Returns to the line it has stopped collecting the index being comparised
        fscanf(raw, "%llu %llu %lf\n", &iaux, &jaux, &value);
        save_line2 = ftell(raw);
        fseek(raw, save_line1, SEEK_SET); //Returns to the line the matrix starts being described 

        for(uint64_t j = 0; j < M; j++){
            fscanf(raw, "%llu %llu %lf\n", &iaa, &jaa, &vaa);
            /*If the index captured by iaa is equal to the one captured by iaux and that line hasn't been checked yet,
            increase the rowsize of the current row*/
            if(iaa == iaux && !rowcheck[iaux]) rownumbers[iaux]++; 
        }
        rowcheck[iaux] = 1; //Checks if that line element has been already read
    }


    for(uint64_t i = 0; i < N; i++){
        m->rowsize[i] = rownumbers[i];
        rowcheck[i] = 0;
    }

    /*SECOND PART - Collecting the actual row values*/
    fseek(raw, save_line1, SEEK_SET);
    save_line2 = ftell(raw);

    uint64_t counter = 0;
    for(uint64_t i = 0; i < M; i++){
        fseek(raw, save_line2, SEEK_SET); //Returns to the line it has stopped collecting the index being comparised
        fscanf(raw, "%llu %llu %lf\n", &iaux, &jaux, &value);
        // printf("%llu, %llu, %lf\n", )
        save_line2 = ftell(raw);
        fseek(raw, save_line1, SEEK_SET); //Returns to the line the matrix starts being described 


        for(uint64_t j = 0; j < M; j++){
            fscanf(raw, "%llu %llu %lf\n", &iaa, &jaa, &vaa);

            if(iaa == iaux && !rowcheck[iaux]){
                m->nodelist[j] = startMatrixNode(vaa, iaux, jaa);
                counter++;
            }

        }

        counter = 0;
        rowcheck[iaux] = 1; //Checks if that line element has been already read
    }
 
    fclose(raw);


    return m;
}



void printMatrix(matrix m){

    uint64_t counter = 0;

    for(uint64_t i = 0; i < m.N; i++){
        char line[500];
        for(uint64_t j = 0; j < m.rowsize[i]; j++){
            printf("%lf ", m.nodelist[counter + j]->value);
        }
        counter += m.rowsize[i];
        sprintf(line, "%s", "\n");
        printf("%s\n", line);
    }

}



double *randArray(uint64_t size, int seed){
    srand(seed); //Set seed for random generator

    double *array = (double *) malloc(size*sizeof(double));
    if(array == NULL){
        fprintf(stderr, "Could not malloc array!\n");
        return NULL;
    }
    else{
        for(uint64_t i = 0; i < size; i++){
            array[i] = pow(-1.0, rand())*rand()/RAND_MAX;
        }

        return array;
    }
}

double *zeroArray(uint64_t size){
    double *array = (double *) malloc(size*sizeof(double));

    if(array == NULL){
        fprintf(stderr, "Could not malloc array!\n");
        return NULL;
    }
    else{
        for(uint64_t i = 0; i < size; i++){
            array[i] = 0;
        }
        return array;
    }
}

void printArray(uint64_t size, double *array){


    printf("[ ");
    for(uint64_t i = 0; i < size - 1; i++){
        printf("%.15lf, ", array[i]);
    }
    printf("%.15lf ]\n", array[size - 1]);
}


void subArray(uint64_t size, double *a, double *b, double *result){
    //result = a - b 
    for(uint64_t i = 0; i < size; i++)
        result[i] = a[i] - b[i];

}

void numbXarray(uint64_t size, double *a, double numb, double *result){

    for(uint64_t i = 0; i < size; i++)
        result[i] = numb*a[i];

}

void copyArray(uint64_t size, double *array, double *result){
    for(uint64_t i = 0; i < size; i++)
        result[i] = array[i];
}


void MatArrayMult(matrix m, double *array, double *result){

    //Auxiliary array to store the result to avoid storing the below results on the array thats being accounted on
    double aux[m.N];
    double r = 0.0;

    for(uint64_t i = 0; i < m.N; i++)
        aux[i] = 0.0;
    
    uint64_t counter = 0;

    for(uint64_t i = 0; i < m.N; i++){
        r = 0.0;
        // printf("%llu\n", m.rowsize[i]);
        for(uint64_t j = 0; j < m.rowsize[i]; j++){
            printf("%lf * %lf\n\n", m.nodelist[counter + j]->value, array[m.nodelist[counter + j]->j]);
            printf("antes %llu | %lf\n", i, r);
            r += m.nodelist[counter + j]->value*array[m.nodelist[counter + j]->j];
            printf("depoi %llu | %lf\n", i, r);
        }
        counter += m.rowsize[i];
        aux[i] = r;
        printf("array = "); printArray(m.N, aux); printf("\n"); 
    }
    // printf("\n");

    //Copying the auxiliary array values to the result array
    for(uint64_t i = 0; i < m.N; i++)
        result[i] = aux[i];

}


double norm(uint64_t size, double *array){

    double n = 0;

    for(uint64_t i = 0; i < size; i++)
        n += array[i]*array[i];

    return sqrt(n);
}

void normalize(uint64_t size, double *array, double *result){
    double nor = norm(size, array);
    for(uint64_t i = 0; i < size; i++)
        result[i] = array[i]/nor;
}



int writeToFile(char *filename, double eigenvalue, double *eigenvector, uint64_t matrix_size){
    
    FILE *out;
    int try = fopen_s(&out, filename, "w");

    if(try != 0){
        fprintf(stderr, "Could not create output file! %d\n", try);
        return BAD_FILE;
    }

    fprintf(out, "%.15lf", eigenvalue); fprintf(out, "\n");
    fprintf(out, "%llu", matrix_size); fprintf(out, "\n");
    for(uint64_t i = 0; i < matrix_size; i++){
        fprintf(out, "%.15lf", eigenvector[i]); fprintf(out, "\n");
    }

    fclose(out);

    return SUCESS;
}

void checkSign(matrix *m, double *array, double *eigenvalue){
    double *copy = zeroArray(m->N);
    double *copy2 = zeroArray(m->N);
    double aux;
    uint64_t index;

    copyArray(m->N, array, copy2);
    MatArrayMult(*m, array, copy);

    //making the array values absolute so we can get the further form zero value
    for(uint64_t i = 0; i < m->N - 1; i++){
        copy2[i] = fabs(copy2[i]);
    }


    //bubble sorting to get the higher absolute value
    for(uint64_t i = 0; i < m->N - 1; i++){
        for(uint64_t j = 0; j < m->N - 1; j++){
            if(copy2[j + 1] < copy2[j]) {
                aux = copy2[j + 1]; 
                copy2[j + 1] = copy2[j]; 
                copy2[j] = aux;
            }
        }
    }


    //Storing the index of the higher value
    for(uint64_t i = 0; i < m->N - 1; i++){
        if(copy2[m->N -1] == fabs(array[i])) index = i;
    }

    copyArray(m->N, array, copy2);

    //If there's a sign change, then the eigenvalue is negative
    if(copy[index]*copy2[index] < 0.0) *eigenvalue = -*eigenvalue;

    free(copy);
    free(copy2);
}