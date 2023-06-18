#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char **argv) {

    int rank, ranksent, size, source, dest, tag, i;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[100], B[100];
    MPI_Barrier(MPI_COMM_WORLD);

    int theSize = 10;

    for (int i = 0; i < theSize; i++) {
        // zad1
        // A[i] = 100 * rank + i;
        // B[i] = rank;
        // zad2
//        A[i] = (10 * (rank + 1)) + i + 1;
//        B[i] = 0;

        A[i] = 100 + 10 * rank + i;
        B[i] = 200 + 10 * rank + i;
    }
    printf("Przed Allonall:\n");
    printf("A[%d]:", rank);
    for (int j = 0; j < theSize; j++) {
        printf(" %d ", A[j]);
    }
    printf("\n");
    printf("B[%d]:", rank);
    for (int j = 0; j < theSize; j++) {
        printf(" %d ", B[j]);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    printf("\n");

    // zad1
    // MPI_Alltoall(&A[1], 2, MPI_INT, &B[0], 2, MPI_INT, MPI_COMM_WORLD);
    // zad2
     MPI_Alltoall(&B[3], 2, MPI_INT, &A[2], 2, MPI_INT, MPI_COMM_WORLD);

    // test
    // rozglaszanie jeden do wszystkich, bierze A[1] z 1 procesu i przypisuje go reszcie
    // MPI_Bcast(&A[1], 1, MPI_INT, 0, MPI_COMM_WORLD);


    // zbieranie wszyscy do jednego bierze A[1] z każdego procesu i przypisuje go tylko pierwszemu procesowi
    // MPI_Gather(&A[1], 1, MPI_INT, &B[2], 1, MPI_INT, 0, MPI_COMM_WORLD);

    // zbieranie wszyscy do wszystkich – równoważne rozgłaszaniu wszyscy do wszystkich, dziala chyba tak samo jak Alltoall
    // MPI_Allgather(&A[1], 1, MPI_INT, &B[2], 1, MPI_INT, MPI_COMM_WORLD);

    // rozpraszanie jeden do wszystkich, jak dla mnie dziala tak samo jak Bcast, nie wiem czym sie rozni
    //if(rank == 0)

    //MPI_Scatter(A, 2, MPI_INT, B, 3, MPI_INT, 2, MPI_COMM_WORLD);


    //MPI_Allreduce(A,B,1,MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    //MPI_Allgather(A, 1, MPI_INT, B, 1, MPI_INT, MPI_COMM_WORLD);

//    else
//        MPI_Scatter(NULL, 2, MPI_INT, &B[0], 2, MPI_INT, 0, MPI_COMM_WORLD);

    printf("---------------------------\n");
    printf("Po Allonall:\n");
    printf("A[%d]:", rank);
    for (int j = 0; j < theSize; j++) {
        printf(" %d ", A[j]);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    printf("\n");
    printf("B[%d]:", rank);
    for (int j = 0; j < theSize; j++) {
        printf(" %d ", B[j]);
    }
    printf("\n\n");



    MPI_Finalize();

    return 0;
}