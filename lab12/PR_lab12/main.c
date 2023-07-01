#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, rankSent, size, source, dest, tag, i;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[100], B[100];
    MPI_Barrier(MPI_COMM_WORLD);

    int theSize = 10;

    for (int i = 0; i < theSize; i++)
    {
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

    // MPI_Alltoall(&A[1], 2, MPI_INT, &B[0], 2, MPI_INT, MPI_COMM_WORLD);

    MPI_Alltoall(&B[3], 2, MPI_INT, &A[2], 2, MPI_INT, MPI_COMM_WORLD);

    // MPI_Bcast(&A[1], 1, MPI_INT, 0, MPI_COMM_WORLD);

    // MPI_Gather(&A[1], 1, MPI_INT, &B[2], 1, MPI_INT, 0, MPI_COMM_WORLD);

    // MPI_Allgather(&A[1], 1, MPI_INT, &B[2], 1, MPI_INT, MPI_COMM_WORLD);

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