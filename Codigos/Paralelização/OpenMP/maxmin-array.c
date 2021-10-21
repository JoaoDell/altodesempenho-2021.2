#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Error { SUCCESS, BAD_ARGUMENT };

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr,
            "Give the desired size of the array in the command line.\n");
    return BAD_ARGUMENT;
  }

  size_t N;
  int n_read_OK = sscanf(argv[1], "%zu", &N);

  if (n_read_OK != 1 || N < 1) {
    fprintf(stderr, "The number of elements must be positive.\n");
    return BAD_ARGUMENT;
  }

  // Random number generation.
  srand(time(NULL));

  // Create a random array of N int elements.
  int *array = (int *)malloc(N * sizeof(int));

  for (size_t i = 0; i < N; ++i) {
    array[i] = rand();
  }

  int max, min;

#pragma omp parallel default(none) shared(max, min, N, array) num_threads(2)
  {
#pragma omp sections
    {
#pragma omp section
      { // First section computes minimum.
        min = array[0];
        for (size_t i = 1; i < N; ++i) {
          if (array[i] < min) {
            min = array[i];
          }
        }
      }

#pragma omp section
      { // Second section computes maximum.
        max = array[0];
        for (size_t i = 1; i < N; ++i) {
          if (array[i] > max) {
            max = array[i];
          }
        }
      }
    } // End of sections. Synchronize threads.
  }   // End of parallel execution.

  printf("Generated %zu random numbers.\n"
         "The Smallest is %d and the largest is %d.\n",
         N, min, max);

  return 0;
}
