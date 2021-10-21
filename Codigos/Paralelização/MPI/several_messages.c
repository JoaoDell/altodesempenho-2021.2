#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  const int MAX = 256;      // Max message size.
  const int TAG_BASE = 100; // Base value for the tags.

  MPI_Init(&argc, &argv);

  int nprocs, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  char message[MAX];

  if (rank == 0) {
    // Process 0 receives the messages.
    int mess_size;

    // Receive one message from each of the other processes.
    for (int i = 1; i < nprocs; i++) {
      MPI_Status status;

      MPI_Recv(message, MAX, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
               MPI_COMM_WORLD, &status);
      // MPI_Recv(message, MAX, MPI_CHAR, i, i + TAG_BASE, MPI_COMM_WORLD, &status);

      MPI_Get_count(&status, MPI_CHAR, &mess_size);

      printf("Message received from %d with tag %d and size %d: %s\n",
             status.MPI_SOURCE, status.MPI_TAG, mess_size, message);
    }
  } else {
    // Other processes send a message to rank 0.
    sprintf(message, "Hello from your %s friend, process %d",
            (rank % 2 == 0) ? "best" : "good", rank);

    // O tag nao precisa variar com o rank.
    // Esta assim apenas para demonstrar MPI_ANY_TAG.
    MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, rank + TAG_BASE,
             MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
