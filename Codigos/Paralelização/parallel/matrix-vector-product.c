#include "simple_matrix.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

enum Error { SUCCESS, BAD_ARGUMENT };

double *matrix_vector_product(Matrix m, const double *v);

inline double rand_uniform() { return (double)rand() / (RAND_MAX + 1.0); }

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
  if (n_read_OK != 1 || num_threads < 1) {
    fprintf(stderr, "The number of threads must be positive.\n");
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

  omp_set_num_threads(num_threads);

  struct timeval t1, t2;

  gettimeofday(&t1, NULL);

  double *vres = matrix_vector_product(m, v);

  double s = 0.0;
#pragma omp parallel for default(none) shared(vres, N) reduction(+ : s)
  for (size_t i = 0; i < N; i++) {
    s += vres[i];
  }

  gettimeofday(&t2, NULL);

  printf("The sum of the resulting vector is %15.8f\n", s);
  printf("It took %f seconds.\n",
         (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6);

  free(vres);
  free(v);
  FREE(m);

  return SUCCESS;
}

double *matrix_vector_product(Matrix m, const double *v) {
  double *res = (double *)malloc(m.nrows * sizeof(double));

#pragma omp parallel for default(none) shared(res, m, v) schedule(static)
  for (size_t i = 0; i < m.nrows; ++i) {
    double s = 0.0;
#pragma omp parallel for default(none) shared(m, v, i) reduction(+ : s)     \
    schedule(static)
    for (size_t j = 0; j < m.ncols; ++j) {
      s += IDX(m, i, j) * v[j];
    }
    res[i] = s;
  }

  return res;
}
