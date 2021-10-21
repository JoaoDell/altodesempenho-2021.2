#include <stdio.h>

// Sharing without race.

enum Error { SUCCESS, BAD_ARGUMENT };

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Give two integer values in the command line.\n");
    return BAD_ARGUMENT;
  }

  int a, b;
  int n_read_OK;
  n_read_OK = sscanf(argv[1], "%d", &a);
  n_read_OK += sscanf(argv[2], "%d", &b);
  if (n_read_OK != 2) {
    fprintf(stderr, "Wrong value for an argument.\n");
    return BAD_ARGUMENT;
  }
  
  int max, min;

#pragma omp parallel default(none) shared(max, min, a, b) num_threads(2)
  {
#pragma omp sections
    {
#pragma omp section
      { // Start first section
        if (a < b) {
          min = a;
        } else {
          min = b;
        }
      } // End of first section.

#pragma omp section
      { // Start second section
        if (a > b) {
          max = a;
        } else {
          max = b;
        }
      } // End of second section.
    }   // End of sections. Synchronize threads.
  }     // End of parallel execution.

  printf("Smallest is %d, largest is %d.\n", min, max);

  return SUCCESS;
}
