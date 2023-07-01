#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int main(void)
{
    int max_number_of_terms = 0;
    int rank, size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        max_number_of_terms = 1000;
    }

    MPI_Bcast(&max_number_of_terms, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int block_size = max_number_of_terms / size;
    int my_start = rank * block_size;
    int my_end = (rank + 1) * block_size - 1;
    int my_stride = 1;

    if (rank == size - 1) {
        my_end = max_number_of_terms - 1;
    }

// Formula: PI/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 etc.
    double suma_plus = 0.0;
    double suma_minus = 0.0;
    int i = 0;
    for (i = my_start; i <= my_end; i += my_stride) {
        int j = 1 + 4 * i;
        suma_plus += 1.0 / j;
        suma_minus += 1.0 / (j + 2.0);
    }
    double pi_approx = 4 * (suma_plus - suma_minus);

    double *results = NULL;
    if (rank == 0) {
        results = (double *) malloc(size * sizeof(double));
    }

    MPI_Gather(&pi_approx, 1, MPI_DOUBLE, results, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double final_result = 0.0;
        for (i = 0; i < size; i++) {
            final_result += results[i];
        }

        printf("PI calculated: \t\t\t%20.15lf\n", final_result);
        printf("PI from the math library: \t%20.15lf\n", M_PI);
        free(results);
    }

    MPI_Finalize();
    return 0;
}