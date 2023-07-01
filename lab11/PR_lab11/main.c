#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {

    int rank, size, dest, tag, i;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_length;
    MPI_Get_processor_name(processor_name, &name_length);

    if (size > 1) {

        if (rank != 0) {
            dest = 0;
            tag = 0;
            int length = MPI_MAX_PROCESSOR_NAME;

            MPI_Send(&length, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(processor_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

        } else {

            for (i = 1; i < size; i++) {
                int message_in;
                MPI_Recv(&message_in, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Expected name length: %d\n", message_in);

                MPI_Recv(processor_name, message_in, MPI_CHAR, i,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                printf("Data from the process with rank: %d, running on the processor: %s (status.MPI_SOURCE -> %d) (i=%d)\n",
                       status.MPI_SOURCE, processor_name, status.MPI_SOURCE, i);
            }
        }
    } else {
        printf("Single process with rank: %d (no messages)\n", rank);
    }

    MPI_Finalize();

    return (0);
}