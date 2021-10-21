#ifndef _SIMPLE_MATRIX_
#define _SIMPLE_MATRIX_

#include <stdlib.h>
#include <string.h>

#define DEFMATRIX(type, name)                                                  \
  typedef struct {                                                             \
    type *_data;                                                               \
    size_t nrows, ncols;                                                       \
  } name;

DEFMATRIX(double, MatrixDouble)
DEFMATRIX(float, MatrixFloat)
DEFMATRIX(int, MatrixInt)

typedef MatrixDouble Matrix;

#define IDX(m, i, j) (m)._data[(size_t)(i) * (m).ncols + (size_t)(j)]

#define INIT_MATRIX(mat, nr, nc)                                               \
  {                                                                            \
    (mat).nrows = (nr);                                                        \
    (mat).ncols = (nc);                                                        \
    (mat)._data = malloc((nr) * (nc) * sizeof((mat)._data[0]));                \
  }

#define FREE(m) free((m)._data)

#define ZERO(m)                                                         \
  memset((m)._data, 0, (m).nrows *(m).ncols * sizeof((m)._data[0]))

#endif /* _SIMPLE_MATRIX_ */
