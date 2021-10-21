#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

enum Error { SUCCESS, BAD_ARGUMENT, BAD_RESULT };

// Prototype of the user interface mergesort function.
void mergesort(int *data, size_t N);

int main(int argc, char const *argv[]) {
  // Read command line argument
  if (argc != 2) {
    fprintf(stderr, "Give the number of elements in the command line.\n");
    return BAD_ARGUMENT;
  }

  size_t N;
  int n_read_OK = sscanf(argv[1], "%zu", &N);
  if (n_read_OK != 1 || N < 1) {
    fprintf(stderr, "Invalid number of elements.\n");
    return BAD_ARGUMENT;
  }

  // Create a random int array
  int *data = (int *)malloc(N * sizeof(int));

  srand(time(NULL));

  for (size_t i = 0; i < N; ++i) {
    data[i] = rand();
  }

  struct timeval t1, t2;
  gettimeofday(&t1, NULL);

  mergesort(data, N);

  gettimeofday(&t2, NULL);

  for (size_t i = 0; i < N - 1; ++i) {
    if (data[i] > data[i + 1]) {
      fprintf(stderr, "Something wrong with the sorting.\n");
      return BAD_RESULT;
    }
  }

  printf("Sorting was successful!\n");
  printf("It took %f milliseconds.\n",
         (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6);

  return SUCCESS;
}

// Implementation of the mergesort and auxiliary functions.

// Merge two sorted sequences in one:
// First sequence is from start (included) to middle (excluded)
// Second sequence if from middle (include) to finish (excluded)
// Results will be in place.
// Pre-condition: start < middle < finish
void merge(int *start, int *middle, int *finish) {
  size_t N = finish - start;

  int *buffer = (int *)malloc(N * sizeof(int));

  int *pb = buffer;
  int *current_left = start;
  int *current_right = middle;

  while (current_left != middle && current_right != finish) {
    if (*current_left <= *current_right) {
      *pb++ = *current_left++;
    } else {
      *pb++ = *current_right++;
    }
  }

  while (current_left != middle) {
    *pb++ = *current_left++;
  }

  while (current_right != finish) {
    *pb++ = *current_right++;
  }

  memcpy(start, buffer, N * sizeof(int));

  free(buffer);
}

// Sort data using mergesort from start (included) to finish (excluded).
void mergesort_rec(int *start, int *finish) {
  size_t N = finish - start;
  if (N > 1) {
    int *middle = start + N / 2;
    // Invariant: middle > start && finish > middle
    mergesort_rec(start, middle);
    mergesort_rec(middle, finish);
    merge(start, middle, finish);
  }
}

// This is the end user interface. Just adjust parameters.
void mergesort(int *data, size_t N) { mergesort_rec(data, data + N); }
