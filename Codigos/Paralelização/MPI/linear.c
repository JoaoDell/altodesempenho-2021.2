#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int nprocs, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Computes the rank of the process at the left.
  int left = (rank != 0) ? rank - 1 : MPI_PROC_NULL;

  // Computes the rank of the process at the right.
  int right = (rank != nprocs - 1) ? rank + 1 : MPI_PROC_NULL;

  // Compute the local value.
  int my_value = rank * rank;

  // Variables to receive values from neighbours.
  int from_left = -1, from_right = -1;

  MPI_Status status;

  MPI_Sendrecv(&my_value, 1, MPI_INT, right, 1, &from_left, 1, MPI_INT, left, 1,
               MPI_COMM_WORLD, &status);
  MPI_Sendrecv(&my_value, 1, MPI_INT, left, 2, &from_right, 1, MPI_INT, right,
               2, MPI_COMM_WORLD, &status);

  printf("Rank %4d: from left: %4d, my value: %4d, from right: %4d.\n",
         rank, from_left, my_value, from_right);

  MPI_Finalize();
  return 0;
}
