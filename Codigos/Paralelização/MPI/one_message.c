#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  const int MAX = 256; // Max message size
  const int TAG = 1;   // Tag for send/receive

  MPI_Init(&argc, &argv);

  int nprocs, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (nprocs != 2) {
    // This code runs only on 2 processes.
    if (rank == 0) {
      fprintf(stderr, "This code only runs with 2 processes.\n");
    }
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  // Running in two processes. Proceed.

  char message[MAX];

  if (rank == 0) {
    // Process 0 executes this.
    MPI_Status status;

    MPI_Recv(message, MAX, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, &status);

    printf("Message received: %s\n", message);
  } else {
    // Process 1 executes this.
    sprintf(message, "Hello from your friend, process %d!\n", rank);

    MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
