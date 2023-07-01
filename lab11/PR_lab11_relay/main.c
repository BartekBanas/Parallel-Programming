#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int prev = rank - 1;
    int next = rank + 1;
    if (prev < 0) prev = size - 1;  // if process rank is 0, the last process is the predecessor
    if (next == size) next = 0;     // if the rank of the process is last, the first process is succeeded

    // variables to store messages received from the predecessor and sent to the successor
    int message_in, message_out;

    // If the process is first in the ring (rank = 0), send a message to the successor
    if (rank == 0)
    {
        message_out = 0;

        MPI_Send(&message_out, 0, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("The process %d sent the number %d to the process %d\n", rank, message_out, next);
    }
    else
    {
        MPI_Recv(&message_in, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received number %d from process %d\n", rank, message_in, prev);
        message_out = message_in + 1;

        if (rank != size - 1) {
            MPI_Send(&message_out, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            printf("The process %d sent the number %d to the process %d\n", rank, message_out, next);
        }
    }

    MPI_Finalize();

    return 0;
}