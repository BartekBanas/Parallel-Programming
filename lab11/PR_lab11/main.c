#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {

    int rank, rankSent, size, source, dest, tag, i;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Print the number of processes
    //printf("Number of processes: %d\n", num_processes);

    // Pobierz nazwę procesora, na którym dany proces jest uruchomiony
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_length;
    MPI_Get_processor_name(processor_name, &name_length);

    if (size > 1) {

        if (rank != 0) {
            dest = 0;
            tag = 0;
            int lenght = MPI_MAX_PROCESSOR_NAME;

            MPI_Send(&lenght, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(processor_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

        } else {

            for (i = 1; i < size; i++) {
                int message_in;
                MPI_Recv(&message_in, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Dlugosc nazwy do przyjecia: %d\n", message_in);

                MPI_Recv(processor_name, message_in, MPI_CHAR, i,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                printf("Dane od procesu o randze: %d, uruchomionego na procesorze: %s (status.MPI_SOURCE -> %d) (i=%d)\n",
                       status.MPI_SOURCE, processor_name, status.MPI_SOURCE, i);
            }
        }
    } else {
        printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
    }


    MPI_Finalize();

    return (0);
}