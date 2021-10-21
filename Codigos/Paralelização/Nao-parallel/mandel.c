#include "simple_matrix.h"
#include <complex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

enum Error { SUCCESS, BAD_ARGUMENT, BAD_FILE };

typedef double complex Point;
typedef size_t Resolution[2];

DEFMATRIX(uint8_t, Image)

// Write image to a file.
int write_image(const char *filename, Image img);

// Read command line arguments or abort.
// Reads: lower left point, upper right point, resolution and output file name.
void read_arguments_or_abort(int argc, char *argv[], Point *lower_left,
                             Point *upper_right, Resolution *resolution,
                             char **filename);

// Calcula a imagem de Mandelbrot para regiao delimitada por
// lower_left e upper_right, com a resolucao especificada.
// Computes and returns the Mandelbrot image delimited by lower_left and
// upper_right and with a given resolution.
Image mandel_block(Point lower_left, Point upper_right, Resolution resolution);

/* Computes the Mandelbrot image for a region and resolution given in the
 * command line arguments. Writes to a file also specified in the command line
 * arguments. */
int main(int argc, char *argv[]) {
  // Read command line arguments.
  Point lower_left, upper_right;
  Resolution resolution;
  char *filename;
  read_arguments_or_abort(argc, argv, &lower_left, &upper_right, &resolution,
                          &filename);

  // Compute the image.
  Image image = mandel_block(lower_left, upper_right, resolution);

  int status = write_image(filename, image);

  FREE(image);

  return status;
}

// Computes the number of Mandelbrot iterations for point c, limited to
// iter_max.
uint8_t mandel(Point c, uint8_t iter_max) {
  uint8_t iter = 0;
  Point z = c;

  while (iter < iter_max && cabs(z) <= 2.0) {
    ++iter;
    z = z * z + c;
  }

  return iter;
}

Image mandel_block(Point lower_left, Point upper_right, Resolution resolution) {
  double hx = (creal(upper_right) - creal(lower_left)) / resolution[0];
  double hy = (cimag(upper_right) - cimag(lower_left)) / resolution[1];

  Image image;
  INIT_MATRIX(image, resolution[1], resolution[0]);

  for (size_t row = 0; row < resolution[1]; ++row) {
    double y = cimag(lower_left) + row * hy + hy / 2;
    for (size_t col = 0; col < resolution[0]; ++col) {
      double x = creal(lower_left) + col * hx + hx / 2;
      IDX(image, row, col) = mandel(x + y * I, 127);
    }
  }

  return image;
}

void read_arguments_or_abort(int argc, char *argv[], Point *lower_left,
                             Point *upper_right, Resolution *resolution,
                             char **filename) {
  if (argc < 8) {
    fprintf(stderr,
            "usage: %s <lowerleftreal> <lowerleftimag> <upperrightreal> "
            "<upperrightimag> <resolution-x> <resolution-y> <outputfilename>\n",
            argv[0]);
    exit(BAD_ARGUMENT);
  }

  int n_read_OK;
  double ar;
  n_read_OK = sscanf(argv[1], "%lf", &ar);
  double ai;
  n_read_OK += sscanf(argv[2], "%lf", &ai);
  double br;
  n_read_OK += sscanf(argv[3], "%lf", &br);
  double bi;
  n_read_OK += sscanf(argv[4], "%lf", &bi);

  if (n_read_OK != 4 || ar >= br || ai >= bi) {
    fprintf(stderr, "Arguments for region delimitation invalid.\n");
    exit(BAD_ARGUMENT);
  }

  *lower_left = ar + ai * I;
  *upper_right = br + bi * I;

  size_t inx;
  n_read_OK = sscanf(argv[5], "%zu", &inx);
  size_t iny;
  n_read_OK += sscanf(argv[6], "%zu", &iny);

  if (n_read_OK != 2 || inx < 1 || iny < 1) {
    fprintf(stderr, "Arguments for image resolution are invalid.\n");
    exit(BAD_ARGUMENT);
  }

  (*resolution)[0] = inx;
  (*resolution)[1] = iny;

  *filename = argv[7];
}

int write_image(const char *filename, Image img) {
  FILE *output = fopen(filename, "w");
  if (output == NULL) {
    perror("Could not open output file.\n");
    return BAD_FILE;
  }

  for (size_t i = 0; i < img.nrows; ++i) {
    for (size_t j = 0; j < img.ncols; ++j) {
      if (fprintf(output, "%4d", IDX(img, i, j)) < 0) {
        fprintf(stderr, "Error writing to output file %s\n", filename);
        fclose(output);
        return BAD_FILE;
      }
    }
    fprintf(output, "\n");
  }
  fclose(output);
  return SUCCESS;
}
