#include<stdio.h>
#include<omp.h>

double funkcja(double x);
void Results();

double funkcja(double x) { return (x * x * x); }

int main(int argc, char *argv[])
{
    omp_set_num_threads(16);

    printf("\nProgram obliczania calki metoda trapezow.\n");

    double a = 0.0;
    double b = 1.0;
    int N = 100000000;
    double dx_adjust = (b - a) / N;

    printf("\nPoczatek obliczen OpenMP\n");
    double t1 = omp_get_wtime();
    int i;
    double calka = 0.0;
#pragma omp parallel for default(none) firstprivate(N, a, dx_adjust) reduction(+:calka)
    for (i = 0; i < N; i++) {

        double x1 = a + i * dx_adjust;
        calka += 0.5 * dx_adjust * (funkcja(x1) + funkcja(x1 + dx_adjust));

        //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n",
        //     i, x1, funkcja(x1), całka);

    }
    t1 = omp_get_wtime() - t1;
    printf("\tCzas wykonania %lf. \tObliczona calka = %.15lf\n\n\n", t1, calka);

    for (int j = 0; j < 5; ++j) {
        Results();
    }
}

void Results()
{
    double a = 0.0;
    double b = 1.0;
    int N = 100000000;
    double dx_adjust = (b - a) / N;

    double t1 = omp_get_wtime();
    int i;
    double calka = 0.0;
#pragma omp parallel for default(none) firstprivate(N, a, dx_adjust) reduction(+:calka)
    for (i = 0; i < N; i++) {

        double x1 = a + i * dx_adjust;
        calka += 0.5 * dx_adjust * (funkcja(x1) + funkcja(x1 + dx_adjust));

        //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n",
        //     i, x1, funkcja(x1), całka);

    }
    t1 = omp_get_wtime() - t1;
    printf("%lf\n", t1);
}