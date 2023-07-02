#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

//#define DIMENSION 19200
//#define SIZE 368640000

#define DIMENSION 12144
#define SIZE (DIMENSION*DIMENSION)

void mat_vec(double *a, double *x, double *y, int n, int nt);

int main(int argc, char **argv)
{
    static double x[DIMENSION], y[DIMENSION], z[DIMENSION];
    double *a;
    int n, nt, i, j;

    double t_begin, t_end;

    int rank, size, source, dest, tag = 0;
    int row, row_last;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    n = DIMENSION;
    int width = n;
    int height = n;

    if (rank == 0)
    {
        a = (double *) malloc(SIZE * sizeof(double));

        for (i = 0; i < SIZE; i++) a[i] = 1.0 * i;
        for (i = 0; i < DIMENSION; i++) x[i] = 1.0 * (DIMENSION - i);

        // for testing - result in y (number of threads switched off)
        mat_vec(a, x, y, n, 4);

    }

    //for(i=0;i<DIMENSION;i++) printf("1 rank %d, x[%2d] = %lf\n",rank,i,x[i]);

    if (size <= 1)
    {
        t_begin = MPI_Wtime();

        //#pragma omp parallel for default(none)  shared(n,a,x,z) // num_threads(nt)
        for (i = 0; i < n; i++) {
            double t = 0.0;
            int ni = n * i;
            int j;
            for (j = 0; j < n; j++) {
                t += a[ni + j] * x[j];
            }
            z[i] = t; // printf("%lf %lf %lf %lf\n", a[ni],x[0], t, z[i]);
        }
        t_end = MPI_Wtime();

        printf("Sequential version\n");

        printf("EXECUTION TIME: executing standard loop: %lf\n", t_end - t_begin);
        printf("\tNumber of operations %d, PERFORMANCE %lf GFlops\n",
               2 * width * height, 2 * width * height / (t_end - t_begin) * 1e-9);
        printf("\tGBytes transferred to processor %lf, speed %lf GB/s\n",
               2 * width * height * sizeof(float) * 1e-9,
               2 * width * height * sizeof(float) / (t_end - t_begin) * 1e-9);

        for (i = 0; i < DIMENSION; i++) {
            if (fabs(y[i] - z[i]) > 1.e-9 * z[i]) printf("Error! i = %d: %lf != %lf\n", i, y[i], z[i]);
        }

    }
    else if (size > 1)
    {
        for (i = 0; i < DIMENSION; i++) z[i] = 0.0;
        MPI_Bcast(&nt, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // podzial wierszowy
        row = ceil(DIMENSION / size);
        row_last = DIMENSION - row * (size - 1);

        if (row != row_last)
        {
            printf("This version does not work with DIMENSION not a multiple of size!\n");
            MPI_Finalize();
            exit(0);
        }

        if (rank == 0)
        {
            //printf("beginning: rank %d size %d\n",rank, size);

            for (i = 1; i < size - 1; i++) {
                MPI_Send(&a[i * DIMENSION * row], row * DIMENSION, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
                MPI_Send(&x[i * row], row, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            }

            MPI_Send(&a[(size - 1) * DIMENSION * row], row_last * DIMENSION, MPI_DOUBLE, size - 1, tag, MPI_COMM_WORLD);
            MPI_Send(&x[(size - 1) * row], row_last, MPI_DOUBLE, size - 1, tag, MPI_COMM_WORLD);

        } else {
            //printf("beginning: rank %d size %d\n",rank, size);

            for (i = 0; i < DIMENSION; i++) x[i] = 0.0;

            source = 0;
            if (rank < size - 1) {

                //printf("before aloc, row %d, rank %d size %d\n",row,rank, size);

                a = (double *) malloc(DIMENSION * row * sizeof(double));

                //printf("after aloc, row %d, rank %d size %d\n",row,rank, size);

                MPI_Recv(a, row * DIMENSION, MPI_DOUBLE, source,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Recv(&x[rank * row], row, MPI_DOUBLE, source,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            } else {

                //printf("before aloc, row %d, rank %d size %d\n",row_last,rank, size);

                a = (double *) malloc(DIMENSION * row * sizeof(double));

                //printf("after aloc, row %d, rank %d size %d\n",row_last,rank, size);

                MPI_Recv(a, row_last * DIMENSION, MPI_DOUBLE, source,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Recv(&x[(size - 1) * row], row_last, MPI_DOUBLE, source,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }

            //for(i=0;i<DIMENSION;i++) printf("rank %d, received x[%2d] = %lf\n",rank,i,x[i]);

            for (j = 0; j < row; j++) {
                for (i = 0; i < DIMENSION; i++) {
                    //printf("rank %d, received a[%4d] = %lf\n",rank,
                    //     rank*row*DIMENSION+j*DIMENSION+i,a[j*DIMENSION+i]);
                }
            }
        }

        if (rank == 0) t_begin = MPI_Wtime();

        //MPI_Allgather(&x[rank*row], row, MPI_DOUBLE, x, row, MPI_DOUBLE, MPI_COMM_WORLD );
        MPI_Allgather(MPI_IN_PLACE, row, MPI_DOUBLE, x, row, MPI_DOUBLE, MPI_COMM_WORLD);

        //for(i=0;i<DIMENSION;i++) printf("after Allgather: rank %d, x[%2d] = %lf\n",rank,i,x[i]);

        //#pragma omp parallel for default(none)  shared(n, row,a,x,z) // num_threads(nt)
        for (i = 0; i < row; i++) {
            double t = 0.0;
            int ni = n * i;
            int j;
            for (j = 0; j < n; j++) {
                t += a[ni + j] * x[j];
            }
            z[i] = t;
        }

        //for(i=0;i<DIMENSION;i++) printf("after mult: rank %d, z[%2d] = %lf\n",rank,i,z[i]);

        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0) {
            t_end = MPI_Wtime();
            printf("Parallel version MPI/OpenMP\n");

            printf("EXECUTION TIME: executing standard loop: %lf\n", t_end - t_begin);
            printf("\tNumber of operations %d, PERFORMANCE %lf GFlops\n",
                   2 * width * height, 2 * width * height / (t_end - t_begin) * 1e-9);
            printf("\tGBytes transferred to processor %lf, speed %lf GB/s\n",
                   2 * width * height * sizeof(float) * 1e-9,
                   2 * width * height * sizeof(float) / (t_end - t_begin) * 1e-9);
        }

        if (rank > 0) {

            MPI_Send(z, row, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

        } else {

            for (i = 1; i < size; i++) {
                MPI_Recv(&z[i * row], row, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
            }

            for (i = 0; i < DIMENSION; i++) {
                if (fabs(y[i] - z[i]) > 1.e-9 * z[i]) {
                    printf("Error! i=%d, y[i]=%lf, z[i]=%lf\n", i, y[i], z[i]);
                }
            }
        }
    }

    MPI_Finalize();

    return (0);
}