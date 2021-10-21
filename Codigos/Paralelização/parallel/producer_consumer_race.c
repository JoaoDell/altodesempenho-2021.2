#include <assert.h>
#include <omp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int PRODUCTION_DELAY = 10000000;
const int CONSUMPTION_DELAY = 1000000;

#ifdef DEBUG
const long ACCREPORTSTEPS = 10000000;
#endif

enum Error { SUCCESS, BAD_ARGUMENT };

// A type for the shared buffer.

#define BUFFER_SIZE 10

typedef struct {
  int data[BUFFER_SIZE];
  int n;
#ifdef DEBUG
  long _num_rd_acc, _num_wr_acc, _num_rd, _num_wr;
#endif
} buffer_type;

void buffer_init(buffer_type *buffer);

#ifdef DEBUG
void buffer_report(buffer_type *buffer);
#endif

// Code executed by consumers.
void consumer(buffer_type *buffer);

// Code executed by producers.
void producer(buffer_type *buffer);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Give the number of threads in the command line.\n");
    return BAD_ARGUMENT;
  }

  int num_threads;
  int n_read_OK = sscanf(argv[1], "%d", &num_threads);
  if (n_read_OK != 1 || num_threads < 2) {
    fprintf(stderr,
            "We need at least two threads (one producer, one consumer).\n");
    return BAD_ARGUMENT;
  }

  buffer_type buffer;
  buffer_init(&buffer);

#pragma omp parallel default(none) shared(buffer) num_threads(num_threads)
  {
    int tid = omp_get_thread_num();
    if (tid == 0) {
      consumer(&buffer);
    } else {
      producer(&buffer);
    }
  }

  // The calls above will never return.
  
  return SUCCESS;
}

int get_value(buffer_type *buffer) {
  while (true) {
    // Repeat until there is something in the buffer to get.
#ifdef DEBUG
    // For debug, register number of accesses
    ++buffer->_num_rd_acc;
    // And print occasionally
    buffer_report(buffer);
#endif
    if (buffer->n > 0) {
#ifdef DEBUG
      ++buffer->_num_rd;
#endif
      // Get last value in the buffer.
      buffer->n--;
      // If the code is right, this invariant should hold.
      assert(0 <= buffer->n && buffer->n < BUFFER_SIZE);
      return buffer->data[buffer->n];
    }
  }
}

void solve_problem(int x) {
  // Do something.
  (void)x; // Just to suppress the "unused parameter" warning.
  struct timespec dt = {0, CONSUMPTION_DELAY};
  nanosleep(&dt, NULL);
}

void consumer(buffer_type *buffer) {
  // Forever read a value and use it.
  while (true) {
    // Get value from buffer.
    int value = get_value(buffer);
    // Do some useful work with the value.
    solve_problem(value);
  }
}

void put_value(buffer_type *buffer, int val) {
  // Wait until there is place to put the value.
  while (true) {
#ifdef DEBUG
    // Count access for debugging.
    ++buffer->_num_wr_acc;
    // Print occasionally
    buffer_report(buffer);
#endif
    if (buffer->n < BUFFER_SIZE) {
#ifdef DEBUG
      ++buffer->_num_wr;
#endif
      // There is space. Put value at the end of the buffer.
      buffer->data[buffer->n] = val;
      buffer->n++;
      // If the code is right, this invariant must hold.
      assert(0 < buffer->n && buffer->n <= BUFFER_SIZE);
      return;
    }
  }
}

int compute_problem() {
  static int value = 0;
  struct timespec dt = {0, PRODUCTION_DELAY};
  nanosleep(&dt, NULL);
  return value++;
}

void producer(buffer_type *buffer) {
  // Forever compute a value and insert in the buffer.
  while (true) {
    // Compute a value
    int value = compute_problem();
    // Insert in buffer
    put_value(buffer, value);
  }
}

void buffer_init(buffer_type *buffer) {
  buffer->n = 0;
#ifdef DEBUG
  buffer->_num_rd_acc = buffer->_num_wr_acc = buffer->_num_rd = buffer->_num_wr = 0;
#endif
}

#ifdef DEBUG
void buffer_report(buffer_type *buffer) {
  // Print occasionally
  if ((buffer->_num_rd_acc + buffer->_num_wr_acc) % ACCREPORTSTEPS == 0) {
    printf("current buffer size = %d, rd tries = %ld, wr tries = %ld, rd done "
           "= %ld, wr done = %ld\n",
           buffer->n, buffer->_num_rd_acc, buffer->_num_wr_acc, buffer->_num_rd,
           buffer->_num_wr);
  }
}
#endif
