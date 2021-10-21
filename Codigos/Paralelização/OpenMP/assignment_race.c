#include <assert.h>
#include <stdio.h>
#include <omp.h>

enum Error { SUCCESS, BAD_ARGUMENT };

// Here we have a race condition on the variable value.

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Give the desired number of threads in the command line.\n");
    return BAD_ARGUMENT;
  }

  int n_threads;
  int n_read_OK = sscanf(argv[1], "%d", &n_threads);
  if (n_read_OK != 1 || n_threads <= 0) {
    printf("Number of threads must be positive.\n");
    return BAD_ARGUMENT;
  }

  int value, tid;

#pragma omp parallel default(none) shared(value) private(tid)                  \
    num_threads(n_threads)
  {
    tid = omp_get_thread_num();
    value = 2 * tid;
  }

  // Guess what will be printed.
  printf("The value is %d.\n", value);

  return SUCCESS;
}
