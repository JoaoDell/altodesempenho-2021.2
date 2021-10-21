#include <stdio.h>
#include <omp.h>

// Shows how to create new threads in OpenMP.
// OpenMP uses fork/join parallelism.

int main() {
  // How many processors are there?
  int nprocs = omp_get_num_procs();

  printf("Running on %d processor(s).\n", nprocs);

  // Set number of threads. 4 times the number of processors. You
  // could use instead num_threads(4*nprocs) at the omp parallel
  // pragma below.
  omp_set_num_threads(4 * nprocs);

  int nthreads, tid;

  // Fork a team of threads giving them their own copies of variable tid
#pragma omp parallel default(none) private(tid, nthreads)
  {
    // Obtain and print thread id.
    tid = omp_get_thread_num();
    printf("Hello World from thread %d.\n", tid);

    // Only master thread does this
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("The total current number of threads is %d.\n", nthreads);
    }
  } // All threads join master thread and terminate.

  printf("\nNow there is only one thread.\n");

  return 0;
}
