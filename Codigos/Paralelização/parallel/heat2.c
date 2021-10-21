#include "simple_matrix.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

enum { SUCCESS, BAD_ARGUMENT, BAD_FILE };

/**
 * Function prototypes.
 */

// Read command line arguments or abort.
// Arguments: <mesh size>, <tolerance>, <output file name>
void read_arguments_or_abort(int argc, char *argv[], size_t *N, double *epsilon,
                             char **filename);

// Write contents of matrix m to a file named filename, excluding border cells.
void write_to_file(const char *filename, Matrix m);

/**
 * Computes heat propagation for a square 2D plate surrounded by a heat bath at
 * constant temperature and with a square box in the middle with forcing.
 *
 * Command line arguments are:
 * <mesh size> <tolerance> <output file name>
 */
int main(int argc, char *argv[]) {
  const double outside_temp = 0.;
  const double box_f_value = -100.;

  size_t N;
  double epsilon;
  char *filename;
  read_arguments_or_abort(argc, argv, &N, &epsilon, &filename);

  struct timeval t1, t2;

  gettimeofday(&t1, NULL);

  // Plate temperature values.
  Matrix u;
  INIT_MATRIX(u, N + 2, N + 2);

  ZERO(u);

  // Adjust border (heat bath) values.
#pragma omp parallel for default(none), shared(u, outside_temp, N)
  for (size_t i = 0; i < N + 2; ++i) {
    IDX(u, 0, i) = outside_temp;
    IDX(u, N + 1, i) = outside_temp;
    IDX(u, i, 0) = outside_temp;
    IDX(u, i, N + 1) = outside_temp;
  }

  // Forcing function values.
  Matrix f;
  INIT_MATRIX(f, N + 2, N + 2);
  ZERO(f);

  // Computes the limits of the internal non-zero forcing region.
  size_t box_first = 2 * N / 5 + 1;
  size_t box_last = 3 * N / 5 + 1;

  // Set values inside this region.
#pragma omp parallel for default(none),                                        \
    shared(f, box_f_value, box_first, box_last) collapse(2)
  for (size_t i = box_first; i <= box_last; ++i) {
    for (size_t j = box_first; j <= box_last; ++j) {
      IDX(f, i, j) = box_f_value;
    }
  }

  // An auxiliary temperature matrix.
  Matrix newu;
  INIT_MATRIX(newu, N + 2, N + 2);

  // Set outside (border) values (will not be changed afterwards).
  // Inside values will be computed below.
#pragma omp parallel for default(none), shared(newu, outside_temp, N)
  for (size_t i = 0; i < N + 2; ++i) {
    IDX(newu, 0, i) = outside_temp;
    IDX(newu, N + 1, i) = outside_temp;
    IDX(newu, i, 0) = outside_temp;
    IDX(newu, i, N + 1) = outside_temp;
  }

  double h = 1. / N;

  bool not_done = true;
  double error = 0.0;

#pragma omp parallel default(none),                                            \
    shared(N, u, newu, h, f, epsilon, error, not_done)
  while (not_done) {
    double partial_error = 0.0;

#pragma omp for schedule(static) collapse(2)
    for (size_t i = 1; i <= N; ++i) {
      for (size_t j = 1; j <= N; ++j) {
        IDX(newu, i, j) =
            0.25 * (IDX(u, i, j - 1) + IDX(u, i, j + 1) + IDX(u, i - 1, j) +
                    IDX(u, i + 1, j) - h * h * IDX(f, i, j));
        double this_error = fabs(IDX(newu, i, j) - IDX(u, i, j));
        if (this_error > partial_error) {
          partial_error = this_error;
        }
      }
    }

#pragma omp critical
    error += partial_error;

#pragma omp barrier

#pragma omp single
    {
      not_done = error > epsilon;
      error = 0.0;

      // Swap u and newu (newu is u for the next step).
      double *aux = u._data;
      u._data = newu._data;
      newu._data = aux;
    }

#pragma omp barrier
  }

  gettimeofday(&t2, NULL);

  printf("It took %f seconds.\n",
         (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6);

  write_to_file(filename, u);

  FREE(newu);
  FREE(f);
  FREE(u);

  return SUCCESS;
}

void read_arguments_or_abort(int argc, char *argv[], size_t *N, double *epsilon,
                             char **filename) {
  if (argc != 4) {
    fprintf(stderr, "Use as: %s <mesh size> <tolerance> <filename>\n", argv[0]);
    exit(BAD_ARGUMENT);
  }

  int n_read_OK = sscanf(argv[1], "%zu", N);
  if (n_read_OK != 1 || *N < 1) {
    fprintf(stderr, "Wrong mesh size argument.\n");
    exit(BAD_ARGUMENT);
  }

  n_read_OK = sscanf(argv[2], "%lf", epsilon);
  if (n_read_OK != 1 || *epsilon <= 0) {
    fprintf(stderr, "Wrong tolerance argument.\n");
    exit(BAD_ARGUMENT);
  }

  *filename = argv[3];
}

void write_to_file(const char *filename, Matrix m) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file to write.");
    exit(BAD_FILE);
  }

  for (size_t i = 1; i < m.nrows - 1; ++i) {
    for (size_t j = 1; j < m.ncols - 1; ++j) {
      int n_OK = fprintf(file, "%8.3f", IDX(m, i, j));
      if (n_OK < 0) {
        fprintf(stderr, "Error writing to file.\n");
        fclose(file);
        exit(BAD_FILE);
      }
    }
    fprintf(file, "\n");
  }
  fclose(file);
}
