#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

//#define DIMENSION 6400
//#define SIZE 40960000

//#define DIMENSION 7936
//#define SIZE 62980096
//#define DIMENSION 19200
//#define SIZE 368640000

//#define DIMENSION 40320 // divisible by 128
//#define DIMENSION 20160 // divisible by 64
#define DIMENSION 10080 // divisible by 1,2,3,4,5,6,7,8,9,10,12,14,16,18,20,24,28,30,32,36,40,60
//#define DIMENSION 4800 // max possible for MPI_Alltoall on Estera
#define SIZE (DIMENSION*DIMENSION)

void mat_vec(double *a, double *x, double *y, int n, int nt);

int main(int argc, char **argv) {
    static double x[DIMENSION], y[DIMENSION], z[DIMENSION]; // z is sized for column decomposition
    double *a;
    double t1;
    int n, nt, i, j;


    int rank, size;
    int row_num, row_num_last;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    n = DIMENSION;

    // x is locally initialized to zero
    for (i = 0; i < DIMENSION; i++) x[i] = 0.0;

    if (rank == 0) {

        a = (double *) malloc((SIZE + 1) * sizeof(double));

        for (i = 0; i < SIZE; i++) a[i] = 1.0 * i;
        for (i = 0; i < DIMENSION; i++) x[i] = 1.0 * (DIMENSION - i);

        nt = 1;

        printf("\n\nstart (sequential execution)\n");

        t1 = MPI_Wtime();
        mat_vec(a, x, y, n, nt);
        t1 = MPI_Wtime() - t1;

        printf("\texecution time (disturbed by MPI?): %lf, Gflop/s: %lf, GB/s> %lf\n",
               t1, 2.0e-9 * SIZE / t1, (1.0 + 1.0 / n) * 8.0e-9 * SIZE / t1);

    }

    if (size > 1) {

        /************** || block row decomposition || *******************/

        // z is initialized for all ranks
        for (i = 0; i < DIMENSION; i++) z[i] = 0.0;

        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // row division
        row_num = ceil(DIMENSION / size);
        row_num_last = DIMENSION - row_num * (size - 1);

        // for row_num!=row_num_last arrays should be oversized to avoid problems
        if (row_num != row_num_last) {

            printf("This version does not work with DIMENSION not a multiple of size!\n");
            MPI_Finalize();
            exit(0);

        }

        // local matrices a_local form parts of a big matrix a
        double *a_local = (double *) malloc(DIMENSION * row_num * sizeof(double));
        for (i = 0; i < DIMENSION * row_num; i++) a_local[i] = 0.0;

        // ... collective communication instead of the following point-to-point

        if (rank == 0) {

            for (i = 0; i < DIMENSION * row_num; i++) a_local[i] = a[i];
        }

        MPI_Scatter(a, row_num * DIMENSION, MPI_DOUBLE, a_local,
                    row_num * DIMENSION, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        double *x_local = malloc(row_num * sizeof(double));

        MPI_Scatter(x, row_num, MPI_DOUBLE, x_local,
                    row_num, MPI_DOUBLE, 0, MPI_COMM_WORLD);


        // ....

        // point-to-point not optimal communication
//        if (rank == 0) {
//
//            for (i = 0; i < DIMENSION * row_num; i++) a_local[i] = a[i];
//
//            for (i = 1; i < size - 1; i++) {
//                MPI_Send(&a[i * DIMENSION * row_num], row_num * DIMENSION, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
//                MPI_Send(&x[i * row_num], row_num, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
//            }
//
//
//            MPI_Send(&a[(size - 1) * DIMENSION * row_num], row_num_last * DIMENSION, MPI_DOUBLE, size - 1, tag, MPI_COMM_WORLD);
//            MPI_Send(&x[(size - 1) * row_num], row_num_last, MPI_DOUBLE, size - 1, tag, MPI_COMM_WORLD);
//
////            if (rank == 0) printf("rank %d, a[0] %lf\n", rank, a[0]);
////            if (rank == 0)
////                printf("rank %d, last %d, a[last] %lf\n", rank,
////                       (size - 1) * DIMENSION * row_num + row_num_last * DIMENSION - 1,
////                       a[(size - 1) * DIMENSION * row_num + row_num_last * DIMENSION - 1]);
//
//        } else {
//
//
//            for (i = 0; i < DIMENSION; i++) x[i] = 0.0;
//
//            source = 0;
//            if (rank < size - 1) {
//
//                MPI_Recv(a_local, row_num * DIMENSION, MPI_DOUBLE, source,
//                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//                MPI_Recv(&x[rank * row_num], row_num, MPI_DOUBLE, source,
//                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//
//            } else {
//
//                MPI_Recv(a_local, row_num_last * DIMENSION, MPI_DOUBLE, source,
//                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//                MPI_Recv(&x[(size - 1) * row_num], row_num_last, MPI_DOUBLE, source,
//                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//
//            }
//        }

        if (rank == 0) {
            printf("Starting MPI matrix-vector product with block row decomposition!\n");
            t1 = MPI_Wtime();
        }

        // local parts of x start at rank*row_num
        //MPI_Allgather(&x[rank*row_num], row_num, MPI_DOUBLE, x, row_num, MPI_DOUBLE, MPI_COMM_WORLD );
        MPI_Allgather(x_local, row_num, MPI_DOUBLE, x, row_num, MPI_DOUBLE, MPI_COMM_WORLD);


        for (i = 0; i < row_num; i++) {

            double t = 0.0;
            int ni = n * i;

            for (j = 0; j < n; j++) {
                t += a_local[ni + j] * x[j];
                //if(i==1){
                //  printf("rank %d: row %d, column %d, a %lf, x %lf, current y %lf\n",
                //         rank, i, j, a_local[ni+j], x[j], t);
                //}
            }
            //printf("rank %d: row %d, final y %lf\n", rank, i, t);
            z[i] = t;
        }

        // just to measure time
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            t1 = MPI_Wtime() - t1;
            printf("Wersja rownolegla MPI z dekompozycja wierszowa blokowa\n");
            printf("\tczas wykonania: %lf, Gflop/s: %lf, GB/s> %lf\n",
                   t1, 2.0e-9 * SIZE / t1, (1.0 + 1.0 / n) * 8.0e-9 * SIZE / t1);

        }

        // ... collective communication instead of the following point-to-point

        // ....

        // point-to-point not optimal communication

//        if (rank > 0) {
//
//            MPI_Send(z, row_num, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
//
//        } else {
//
//            for (i = 1; i < size; i++) {
//                MPI_Recv(&z[i * row_num], row_num, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
//
//            }
//        }


        MPI_Allgather(z, row_num, MPI_DOUBLE, z,
                      row_num, MPI_DOUBLE, MPI_COMM_WORLD);

        if (rank == 0) {

            for (i = 0; i < DIMENSION; i++) {
                if (fabs(y[i] - z[i]) > 1.e-9 * z[i]) {
                    printf("Blad! i=%d, y[i]=%lf, z[i]=%lf\n", i, y[i], z[i]);
                }
            }
        }


        /************** || block column decomposition (collective only) || *******************/
        int n_col = row_num; // each process processes ncol columns

        // z is initialized for all ranks
        for (i = 0; i < DIMENSION; i++) z[i] = 0.0;

        // local a is initialized to zero - now local a stores several columns (not rows as before)
        for (i = 0; i < DIMENSION * n_col; i++) a_local[i] = 0.0;

        // for each row (starting at i*DIMENSION - due to row storage)
        for (i = 0; i < DIMENSION; i++) {

            // ... collective communication necessary to distribute data
            // several elements (corresponding to ncol columns) are send to each process

        }

        if (rank == 0) {
            printf("Starting MPI matrix-vector product with block column decomposition!\n");
            t1 = MPI_Wtime();
        }

        // loop over rows (optimal since matrix a stored row major)
        for (i = 0; i < n; i++) {

            double t = 0.0;
            int ni;
            if (rank == 0) {
                // rank==0 stores the whole a - next row starts after n elements
                ni = n_col * i;
            } else {
                // ranks>0 store the parts of a - next row starts after n_col elements
                ni = n_col * i;
            }

            // assuming the whole x stored at all processes (should be of size n_col with jj=0)
            int jj = rank * n_col;
            for (j = 0; j < n_col; j++) {
                t += a_local[ni + j] * x[jj + j];
                //if(i==1){
                //  printf("rank %d: row %d, column %d, a %lf, x %lf, current y %lf\n",
                //	 rank, i, j, a_local[ni+j], x[j], t);
                //}
            }
            //printf("rank %d: row %d, final y %lf\n", rank, i, t);
            z[i] = t;
        }


        // I. Reduce - for each element of z reduction is necessary to get the final result
        // Ia. version with loop over ranks and reductions for n_col long chunks

        // Ib. simple version with reductions for individual elements of z (loop over rows)
        // end I. Reduce

        // II. All-to-all
        // WARNING: All-to-all requires large MPI buffers (check matrix size DIMENSION in case of errors)

        // All-to-all requires also synchronisation
        // MPI_Barrier(MPI_COMM_WORLD);

        // Alltoall
        // ...

        // All-to-all requires synchronisation also at this point
        /* MPI_Barrier(MPI_COMM_WORLD); */

        // Alltoall exchanges chunks of vector z - each process stores nproc chunks
        // with size n_col obtained from other processes, to get the final result
        // at the proper positions from z(rank*n_col) to z(rank*n_col+n_col-1)
        // each process has to sum all nproc chunks (it is assumed that the owned chunk
        // retains the calculated value as the initial value for summation
        // (remark: nproc is equal to size in MPI nomenclature, not to be mixed up
        // with the size of parts of z equal to n_col)

//         for(i=0; i<size; i++){
//           if(i!=rank){
//         	for(j=0;j<n_col;j++){
//         	  z[rank*n_col+j] += z[i*n_col+j];
//         	}
//           }
//         }

        // end II. All-to-all

        // just to measure time
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            t1 = MPI_Wtime() - t1;
            printf("Parallel version of MPI with block column decomposition\n");
            printf("\tExecution time: %lf, Gflop/s: %lf, GB/s> %lf\n",
                   t1, 2.0e-9 * SIZE / t1, (1.0 + 1.0 / n) * 8.0e-9 * SIZE / t1);

        }

        // ... collective communication necessary to collect data for z vector
        // ...


        // testing - switch on after completing calculations and communcation
        if (rank == 0) {

            for (i = 0; i < DIMENSION; i++) {
                if (fabs(y[i] - z[i]) > 1.e-9 * z[i]) {
                    printf("Error! i=%d, y[i]=%lf, z[i]=%lf - complete the code for column decomposition\n",
                           i, y[i], z[i]);
                    break;
                    //printf("Error! i=%d, y[i]=%lf, z[i]=%lf\n",i, y[i], z[i]);
                }
            }
        }

        // the end if size > 1
    }

    MPI_Finalize();
    return (0);
}

// slightly optimized version of matrix-vector product with possible OpenMP parallelization
void mat_vec(double *a, double *x, double *y, int nn, int nt) {

    register int n = nn;
    register int i;
//#pragma omp parallel for num_threads(nt) default(none) shared (a,x,y,n)
    for (i = 0; i < n; i += 2) {
        register double ty1 = 0;
        register double ty2 = 0;
        register int j;
        for (j = 0; j < n; j += 2) {
            register double t0 = x[j];
            register double t1 = x[j + 1];
            register int k = i * n + j;
            ty1 += a[k] * t0 + a[k + 1] * t1;
            ty2 += a[k + n] * t0 + a[k + 1 + n] * t1;
        }
        //printf("nr threads = %d\n", omp_get_num_threads());
        y[i] = ty1;
        y[i + 1] += ty2;
    }
}