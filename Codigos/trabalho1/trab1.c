#include "matrix.h"


int main(){

    matrix *m = getMatrixFromFile("test.dat");

    double *a = randArray(m->N);
    double *result = zeroArray(m->N); 




    for(int n = 0; n < 100; n++){
        normalize(m->N, a, a);
        MatArrayMult(*m, a, a);
        printf("%.15lf ", norm(m->N, a));
        printArray(m->N, a);
    }


    return 0;

}