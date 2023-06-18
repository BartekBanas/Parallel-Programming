#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
// Inicjalizuj MPI
    MPI_Init(&argc, &argv);
// Pobierz rank procesu i liczbę procesów
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

// Określ poprzednika i następcę procesu w pierścieniu
    int prev = rank - 1;
    int next = rank + 1;
    if (prev < 0) prev = size - 1; // jeśli rank procesu jest równy 0, poprzednikiem jest ostatni proces
    if (next == size) next = 0; // jeśli rank procesu jest ostatni, następcą jest pierwszy proces

// zmienne do przechowywania komunikatów otrzymanych od poprzednika i wysyłanych do następcy
    int message_in, message_out;

// Jeśli proces jest pierwszym w pierścieniu (rank = 0), wyślij komunikat do następcy
    if (rank == 0)
    {
        message_out = 0;

        MPI_Send(&message_out, 0, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Proces %d wyslal liczbe\t%d do procesu %d\n", rank, message_out, next);
    }
    else
    {
        MPI_Recv(&message_in, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Proces %d odebral liczbe\t%d od procesu %d\n", rank, message_in, prev);
        message_out = message_in + 1;

        if (rank != size - 1) {
            MPI_Send(&message_out, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            printf("Proces %d wyslal liczbe\t%d do procesu %d\n", rank, message_out, next);
        }
    }
// Każdy proces otrzymuje komunikat od poprzednika i przekazuje go do następcy


// Jeśli proces jest ostatnim w pierścieniu (rank = size - 1),
// wyświetl otrzymany komunikat
//    if (rank == size - 1) {
//        printf("Otrzymano komunikat: %d\n", message_in);
//    }

    MPI_Finalize();

    return 0;
}