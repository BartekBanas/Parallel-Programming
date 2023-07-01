#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<math.h>

#define SIZE 1000000

int main()
{
    int threadsAmount = 11;
    omp_set_num_threads(threadsAmount);

    int i;
    double *A = malloc((SIZE + 2) * sizeof(double));
    double *B = malloc((SIZE + 2) * sizeof(double));
    double *C = malloc((SIZE + 2) * sizeof(double));
    double sum;

    for (i = 0; i < SIZE + 2; i++) A[i] = (double) i / SIZE;
    for (i = 0; i < SIZE + 2; i++) B[i] = 1.0 - (double) i / SIZE;

    double startingTime = omp_get_wtime();
    for (i = 0; i < SIZE; i++) {
        A[i] += A[i + 2] + sin(B[i]);
    }

    startingTime = omp_get_wtime() - startingTime;

    sum = 0.0;
    for (i = 0; i < SIZE + 2; i++) sum += A[i];           // Sequent
    printf("Sum %lf, calculation time %lf\n", sum, startingTime);

    for (i = 0; i < SIZE + 2; i++) A[i] = (double) i / SIZE;
    for (i = 0; i < SIZE + 2; i++) B[i] = 1.0 - (double) i / SIZE;

    sum = 0.0;
    startingTime = omp_get_wtime();

#pragma omp parallel default(none) shared(sum, A, B) num_threads(2)     // Peasant Reduction
#pragma omp for reduction(+:sum)
    for (i = 0; i < SIZE; i++) {
        sum += A[i] + A[i + 2] + sin(B[i]);
    }

    startingTime = omp_get_wtime() - startingTime;
    sum += A[SIZE] + A[SIZE + 1];

    printf("Sum %lf, parallel computation time %lf\n", sum, startingTime);

    for (i = 0; i < SIZE + 2; i++) {                                    // Epic Array independence
        A[i] = (double) i / SIZE;
        C[i] = A[i];
    }

    for (i = 0; i < SIZE + 2; i++) B[i] = 1.0 - (double) i / SIZE;

    startingTime = omp_get_wtime();

#pragma omp parallel for default(none) shared(A, B, C) num_threads(2)
    for (i = 0; i < SIZE; i++) {
        A[i] += C[i + 2] + sin(B[i]);
    }

    startingTime = omp_get_wtime() - startingTime;

    sum = 0.0;

    for (i = 0; i < SIZE + 2; i++) sum += A[i];
    printf("Sum %lf, parallel computation time %lf\n", sum, startingTime);
}