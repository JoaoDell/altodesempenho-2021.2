#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum Error { SUCCESS, BAD_ARGUMENT };

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Give the size of the vectors and the number of threads in "
                    "the command line.\n");
    return BAD_ARGUMENT;
  }

  size_t N;
  int n_read_OK = sscanf(argv[1], "%zu", &N);

  if (n_read_OK != 1 || N < 1) {
    fprintf(stderr, "The number of elements must be positive.\n");
    return BAD_ARGUMENT;
  }

  int num_threads;
  n_read_OK = sscanf(argv[2], "%d", &num_threads);

  if (n_read_OK != 1 || num_threads <= 0) {
    fprintf(stderr, "The number of threads must be positive.\n");
    return BAD_ARGUMENT;
  }

  // Create an array from 0 to N-1.
  int64_t *array = (int64_t *)malloc(N * sizeof(int64_t));
  for (size_t i = 0; i < N; ++i) {
    array[i] = i;
  }

  int64_t sum = 0;

#pragma omp parallel default(none) shared(N, array) reduction(+:sum)     \
    num_threads(num_threads)
  {
#pragma omp for
    for (size_t i = 0; i < N; ++i) {
      sum += array[i];
    }
  } // End of parallel execution.

  int64_t n64 = (int64_t)N;
  if (sum == (n64 * (n64 - 1) / 2)) {
    printf("Nice, I can sum!\n");
  } else {
    printf("Back to school!\n");
  }

  return SUCCESS;
}
