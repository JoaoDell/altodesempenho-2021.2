#ifndef _SIMPLE_MATRIX_
#define _SIMPLE_MATRIX_


#include <stdlib.h>
#include <string.h>

#define DEFMATRIX(type, name) typedef struct{type *_data; size_t ncol, nrow;} name;


DEFMATRIX(double, MatrixDouble);
DEFMATRIX(float, MatrixFloat);
DEFMATRIX(int, MatrixInt);

#define MatrixFloat Matrix

#define IDX(m, i, j) (m)._data[(size_t)(i)*(m).ncol + (size_t)(j)]

#define INIT_MATRIX(mat, nr, nc){(mat).ncol = (nc); (mat).nrow = (nr); (mat)._data = malloc((nr)*(nc)*sizeof((mat)._data[0]))}

#define ZERO(m) memset((m)._data, 0, (m).nrow*(m).ncol*sizeof((m)._data[0]))

#endif