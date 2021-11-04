#include "matrix.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

matrixNode *startMatrixNode(double value, uint32_t i, uint32_t j){

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

void setMatrixNode(matrixNode *mn, double value, uint32_t i, uint32_t j){
    mn->value = value;
    mn->i = i; mn->j = j;
}


matrix *startMatrix(uint32_t N, uint32_t M){

    matrix *mat = (matrix *) malloc(sizeof(matrix));
    if(mat == NULL) {
        fprintf(stderr, "%s", "Matrix could not be allocated!\n");
        return NULL;
    }
    else{
        mat->rowsize = (uint32_t *) malloc(N*sizeof(uint32_t));
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

    uint32_t N, M;


    FILE *raw;
    fopen_s(&raw, filename, "rb");

    char line[500];

    //Gets N and M from file
    fscanf(raw, "%u\n", &N);
    fscanf(raw, "%u\n", &M);


    /*FIRST PART - Collecting the actual size of the rows*/
    uint32_t iaux = 0, jaux = 0;
    double value;

    uint32_t rownumbers[N];
    int rowcheck[N];
    for(uint32_t k = 0; k < N; k++){
        rownumbers[k] = 0;
        rowcheck[k] = 0;
    }

    /*CHECKING HOW LONG ARE EACH MATRIX ROW*/
    long save_line1 = 0;
    save_line1 = ftell(raw);

    for(uint32_t i = 0; i < M; i++){
        fscanf(raw, "%u %u %lf\n", &iaux, &jaux, &value);
        rownumbers[iaux]++;
        if(iaux != jaux) rownumbers[jaux]++;
    }
    /*END CHECKING*/


    //Capturing the actual size of the matrix to be malloced
    uint32_t total = 0;
    for(uint32_t i = 0; i < N; i++)
        total += rownumbers[i];



    matrix *m = startMatrix(N, total);



    uint32_t rowadded[N];

    for(uint32_t i = 0; i < N; i++){
        m->rowsize[i] = rownumbers[i];
        rowcheck[i] = 0;
        rowadded[i] = rownumbers[i];
    }


    /*SECOND PART - Collecting the actual row values*/



    /* TESTE DE ALGORITMO
            
        [9.1, 4.6, 4.4, 7.8, 4.5, 2.0, 4.4 ]
        -------0------|----1----|----2-----|

        rownumbers
        [3, 2, 2]
        rowadded
        [0, 0, 0]  

        iaux = 2
        counter = 5;

        uint32_t offset = 0, offset2 = 0;
        for(uint32_t i = 0; i < M; i++){
            fscanf(raw, "%u %u %lf\n", &iaux, &jaux, &value);
            for(uint32_t j = 0; j < iaux; j++) offset += rownumbers[j];
            m->nodelist[offset + (rownumbers[iaux] - rowadded[iaux])] = startMatrixNode(value, iaux, jaux);

            if(iaux != jaux){
                for(uint32_t j = 0; j < jaux; j++) offset2 += rownumbers[j];
                m->nodelist[offset2 + (rownumbers[jaux] - rowadded[jaux])] = startMatrixNode(value, jaux, iaux);
                rowadded[jaux]--;
            }
            rowadded[iaux]--;
            offset = 0;
            offset2 = 0;
        }
    
    */
    fseek(raw, save_line1, SEEK_SET);
    save_line1 = ftell(raw);


    uint32_t offset = 0, offset2 = 0;
    for(uint32_t i = 0; i < M; i++){
        fscanf(raw, "%u %u %lf\n", &iaux, &jaux, &value);
        for(uint32_t j = 0; j < iaux; j++) offset += rownumbers[j];
        m->nodelist[offset + (rownumbers[iaux] - rowadded[iaux])] = startMatrixNode(value, iaux, jaux);

        if(iaux != jaux){
            for(uint32_t j = 0; j < jaux; j++) offset2 += rownumbers[j];
            m->nodelist[offset2 + (rownumbers[jaux] - rowadded[jaux])] = startMatrixNode(value, jaux, iaux);
            rowadded[jaux]--;
        }
        rowadded[iaux]--;
        offset = 0;
        offset2 = 0;
    }

    fclose(raw);


    return m;
}


void printMatrix(matrix m){

    uint32_t counter = 0;

    for(uint32_t i = 0; i < m.N; i++){
        char line[500];
        for(uint32_t j = 0; j < m.rowsize[i]; j++){
            printf("%lf ", m.nodelist[counter + j]->value);
        }
        counter += m.rowsize[i];
        sprintf(line, "%s", "\n");
        printf("%s\n", line);
    }

}



double *randArray(uint32_t size, int seed){
    srand(seed); //Set seed for random generator

    double *array = (double *) malloc(size*sizeof(double));
    if(array == NULL){
        fprintf(stderr, "Could not malloc array!\n");
        return NULL;
    }
    else{
        for(uint32_t i = 0; i < size; i++){
            array[i] = pow(-1.0, rand())*rand()/RAND_MAX;
        }

        return array;
    }
}

double *zeroArray(uint32_t size){
    double *array = (double *) malloc(size*sizeof(double));

    if(array == NULL){
        fprintf(stderr, "Could not malloc array!\n");
        return NULL;
    }
    else{
        for(uint32_t i = 0; i < size; i++){
            array[i] = 0;
        }
        return array;
    }
}

void printArray(uint32_t size, double *array){
    printf("[ ");
    for(uint32_t i = 0; i < size - 1; i++){
        printf("%.15lf, ", array[i]);
    }
    printf("%.15lf ]\n", array[size - 1]);
}


void subArray(uint32_t size, double *a, double *b, double *result){
    //result = a - b 
    for(uint32_t i = 0; i < size; i++)
        result[i] = a[i] - b[i];

}

void numbXarray(uint32_t size, double *a, double numb, double *result){

    for(uint32_t i = 0; i < size; i++)
        result[i] = numb*a[i];

}

void copyArray(uint32_t size, double *array, double *result){
    for(uint32_t i = 0; i < size; i++)
        result[i] = array[i];
}

void MatArrayMult(matrix m, double *array, double *result){

    //Auxiliary array to store the result to avoid storing the below results on the array thats being accounted on
    double aux[m.N];
    double r = 0.0;

    for(uint32_t i = 0; i < m.N; i++)
        aux[i] = 0.0;
    
    uint32_t counter = 0;

    for(uint32_t i = 0; i < m.N; i++){
        r = 0.0;
        for(uint32_t k = 0; k < m.rowsize[i]; k++){
            r += m.nodelist[counter + k]->value*array[m.nodelist[counter + k]->j];
        }
        counter += m.rowsize[i];
        aux[i] = r;
    }

    //Copying the auxiliary array values to the result array
    for(uint32_t i = 0; i < m.N; i++)
        result[i] = aux[i];

}


double norm(uint32_t size, double *array){

    double n = 0;

    for(uint32_t i = 0; i < size; i++)
        n += array[i]*array[i];

    return sqrt(n);
}

void normalize(uint32_t size, double *array, double *result){
    double nor = norm(size, array);
    for(uint32_t i = 0; i < size; i++)
        result[i] = array[i]/nor;
}



int writeToFile(char *filename, double eigenvalue, double *eigenvector, uint32_t matrix_size){
    
    FILE *out;
    int try = fopen_s(&out, filename, "w");

    if(try != 0){
        fprintf(stderr, "Could not create output file! %d\n", try);
        return BAD_FILE;
    }

    fprintf(out, "%.15lf", eigenvalue); fprintf(out, "\n");
    fprintf(out, "%u", matrix_size); fprintf(out, "\n");
    for(uint32_t i = 0; i < matrix_size; i++){
        fprintf(out, "%.15lf", eigenvector[i]); fprintf(out, "\n");
    }

    fclose(out);

    return SUCESS;
}

void checkSign(matrix *m, double *array, double *eigenvalue){
    double *copy = zeroArray(m->N);
    double *copy2 = zeroArray(m->N);
    double aux;
    uint32_t index;

    copyArray(m->N, array, copy2);
    MatArrayMult(*m, array, copy);

    //making the array values absolute so we can get the further form zero value
    for(uint32_t i = 0; i < m->N - 1; i++){
        copy2[i] = fabs(copy2[i]);
    }


    //bubble sorting to get the higher absolute value
    for(uint32_t i = 0; i < m->N - 1; i++){
        for(uint32_t j = 0; j < m->N - 1; j++){
            if(copy2[j + 1] < copy2[j]) {
                aux = copy2[j + 1]; 
                copy2[j + 1] = copy2[j]; 
                copy2[j] = aux;
            }
        }
    }


    //Storing the index of the higher value
    for(uint32_t i = 0; i < m->N - 1; i++){
        if(copy2[m->N -1] == fabs(array[i])) index = i;
    }

    copyArray(m->N, array, copy2);

    //If there's a sign change, then the eigenvalue is negative
    if(copy[index]*copy2[index] < 0.0) *eigenvalue = -*eigenvalue;

    free(copy);
    free(copy2);
}