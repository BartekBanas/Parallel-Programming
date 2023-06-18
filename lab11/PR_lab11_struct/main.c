#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"mpi.h"

typedef struct {
    int integer;
    double someNumber;
    char miniString[3];
} Data;

int main(int argc, char **argv) {
    int rank, size, i, source, tag, packet_size;
    MPI_Status status;
    Data local_data;
    packet_size = sizeof(Data);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        if (rank != 0) {
            void *buffer = (void *) malloc(sizeof(Data));
            int position = 0;
            local_data.integer = 10 * rank;
            local_data.someNumber = 0.5 * rank;
            strcpy(local_data.miniString, "OK\0");

            MPI_Pack(&local_data.integer, 1, MPI_INT, buffer, packet_size, &position, MPI_COMM_WORLD);
            MPI_Pack(&local_data.someNumber, 1, MPI_DOUBLE, buffer, packet_size, &position, MPI_COMM_WORLD);
            MPI_Pack(&local_data.miniString, 3, MPI_CHAR, buffer, packet_size, &position, MPI_COMM_WORLD);
            MPI_Send(buffer, position, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
            printf("Process %d sent data to %d: int: %d,\tdouble: %lf,\tminiString: %s\n", rank, 0, local_data.integer,
                   local_data.someNumber, local_data.miniString);
            free(buffer);
        } else {
            for (i = 0; i < size - 1; i++) {
                void *buffer = (void *) malloc(sizeof(Data));
                int position = 0;
                MPI_Recv(buffer, packet_size, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Unpack(buffer, packet_size, &position, &local_data.integer, 1, MPI_INT, MPI_COMM_WORLD);
                MPI_Unpack(buffer, packet_size, &position, &local_data.someNumber, 1, MPI_DOUBLE, MPI_COMM_WORLD);
                MPI_Unpack(buffer, packet_size, &position, &local_data.miniString, 3, MPI_CHAR, MPI_COMM_WORLD);

                printf("Process %d received data from %d: int: %d,\tdouble: %lf,\tminiString: %s\n", rank,
                       status.MPI_SOURCE, local_data.integer, local_data.someNumber, local_data.miniString);
                free(buffer);
            }
        }
    }
    MPI_Finalize();
    return 0;
}