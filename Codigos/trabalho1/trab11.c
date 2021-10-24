#include "matrix.h"


int main(int argc, char *argv[]){

    if(argc != 4){
        fprintf(stderr, "Give in the exact order:\n - Source matrix file name\n - Precision\n - Output file name where the results will be written!\n");
        return BAD_ARGUMENT; 
    }

    double precision;
    sscanf(argv[2], "%lf", &precision);
    

    matrix *m = getMatrixFromFile(argv[1]);

    double *a = randArray(m->N, 123);
    double *r1 = zeroArray(m->N);
    double *r2 = zeroArray(m->N); 

    double eigenvalue = norm(m->N, a);
    double last_eigen = 0.0;
    int iter = 0;

    // printMatrix(*m);


    while(iter <= 3 || (eigenvalue - last_eigen)/eigenvalue > precision){
        last_eigen = norm(m->N, a);
        normalize(m->N, a, a);
        printArray(m->N, a);
        printf("%.15lf\n", eigenvalue);
        MatArrayMult(*m, a, a);
        eigenvalue = norm(m->N, a);
        iter++;
    }

    normalize(m->N, a, a);

    //Calculating the value to check whether the accounts make sense or not
    MatArrayMult(*m, a, r1);
    // printf("%lf\n", eigenvalue);
    numbXarray(m->N, a, eigenvalue, r2);
    // printArray(m->N, r1);
    // printArray(m->N, r2);
    subArray(m->N, r1, r2, r1);
    

    printf("%.15lf", norm(m->N, r1)/eigenvalue);

    writeToFile(argv[3], eigenvalue, a, m->N);

    return 0;

}