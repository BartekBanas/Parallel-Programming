#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<math.h>

#define N 1000000

int main() {

    int liczba_watkow = 11;
    omp_set_num_threads(liczba_watkow);

    int i;
    double *A = malloc((N + 2) * sizeof(double));
    double *B = malloc((N + 2) * sizeof(double));
    double *C = malloc((N + 2) * sizeof(double));
    double suma;

    for (i = 0; i < N + 2; i++) A[i] = (double) i / N;
    for (i = 0; i < N + 2; i++) B[i] = 1.0 - (double) i / N;

    double t1 = omp_get_wtime();
    for (i = 0; i < N; i++) {
        A[i] += A[i + 2] + sin(B[i]);
    }

    t1 = omp_get_wtime() - t1;

    suma = 0.0;
    for (i = 0; i < N + 2; i++) suma += A[i];           // Sequent
    printf("suma %lf, czas obliczen %lf\n", suma, t1);


    for (i = 0; i < N + 2; i++) A[i] = (double) i / N;
    for (i = 0; i < N + 2; i++) B[i] = 1.0 - (double) i / N;


    suma = 0.0;
    t1 = omp_get_wtime();

#pragma omp parallel default(none) shared(suma, A, B) num_threads(2)     // Peasant Reduction
#pragma omp for reduction(+:suma)
    for (i = 0; i < N; i++) {
        suma += A[i] + A[i + 2] + sin(B[i]);
    }
    t1 = omp_get_wtime() - t1;
    suma += A[N] + A[N + 1];

    printf("suma %lf, czas obliczen rownoleglych %lf\n", suma, t1);

    for (i = 0; i < N + 2; i++) {                                    // Epic Array independence
        A[i] = (double) i / N;
        C[i] = A[i];
    }
    for (i = 0; i < N + 2; i++) B[i] = 1.0 - (double) i / N;

    t1 = omp_get_wtime();

#pragma omp parallel for default(none) shared(A, B, C) num_threads(2)
    for (i = 0; i < N; i++) {
        A[i] += C[i + 2] + sin(B[i]);
    }

    t1 = omp_get_wtime() - t1;

    suma = 0.0;
    for (i = 0; i < N + 2; i++) suma += A[i];
    printf("suma %lf, czas obliczen rownoleglych %lf\n", suma, t1);
}
