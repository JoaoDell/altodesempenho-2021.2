#include "simple_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Error { SUCCESS, BAD_ARGUMENT };

double *matrix_vector_product(Matrix m, const double *v);

inline double rand_uniform() { return (double)rand() / (RAND_MAX + 1.0); }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Give the size of the vectors in the command line.\n");
    return BAD_ARGUMENT;
  }

  size_t N;
  int n_read_OK = sscanf(argv[1], "%zu", &N);
  if (n_read_OK != 1 || N < 1) {
    fprintf(stderr, "The number of elements must be positive.\n");
    return BAD_ARGUMENT;
  }

  double *v = (double *)malloc(N * sizeof(double));
  Matrix m;
  INIT_MATRIX(m, N, N);

  // Random number generation
  srand(time(NULL));

  for (size_t i = 0; i < N; ++i) {
    v[i] = rand_uniform();
  }

  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      IDX(m, i, j) = rand_uniform();
    }
  }

  double *vres = matrix_vector_product(m, v);

  double s = 0.0;
  for (size_t i = 0; i < N; i++) {
    s += vres[i];
  }

  printf("The sum of the resulting vector is %15.8f\n", s);

  free(vres);
  free(v);
  FREE(m);
  
  return SUCCESS;
}

double *matrix_vector_product(Matrix m, const double *v) {
  double *res = (double *)malloc(m.nrows * sizeof(double));

  for (size_t i = 0; i < m.nrows; ++i) {
    double s = 0.0;
    for (size_t j = 0; j < m.ncols; ++j) {
      s += IDX(m, i, j) * v[j];
    }
    res[i] = s;
  }

  return res;
}
