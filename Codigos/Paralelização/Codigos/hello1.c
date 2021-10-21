#include <stdio.h>
#include <omp.h>


int main(){

    int nprocs = omp_get_num_procs();



    int cthread, nthreads;

    nthreads = nprocs;

    int array[nthreads];

    omp_set_num_threads(nprocs);

#pragma omp parallel default(none) private(cthread, nthreads) shared(array)
    {

        cthread = omp_get_thread_num();

        array[cthread] = cthread;

        // printf("%d ", array[cthread]);
        // printf("Thread %d falando\n", cthread);

        // if(cthread == 0){
        //     nthreads = omp_get_num_threads();
        //     printf("Existem %d threads executando no momento.\n", nthreads);
        // }



    }

    for(int i = 0; i < nprocs; i++){
        printf("%d ", array[i]);
    }

    // printf("Existem %d threads executando no momento.\n", omp_get_num_threads());

}