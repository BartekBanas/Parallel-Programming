#include <stdio.h>
#include <omp.h>

void Results();

double cube(double x) {
    return (x * x * x);
}

int main()
{
    omp_set_num_threads(16);

    printf("\nProgram for calculating the integral using the trapezoidal method\n");

    double a = 0.0;
    double b = 1.0;
    int N = 100000000;
    double dx_adjust = (b - a) / N;

    printf("\nBeginning of OpenMP calculations\n");
    double starting_time = omp_get_wtime();
    int i;
    double integral = 0.0;

#pragma omp parallel for default(none) firstprivate(N, a, dx_adjust) reduction(+:integral)
    for (i = 0; i < N; i++)
    {
        double x1 = a + i * dx_adjust;
        integral += 0.5 * dx_adjust * (cube(x1) + cube(x1 + dx_adjust));

        //printf("i %d, x1 %lf, cube(x1) %lf, integral = %.15lf\n",
        //     i, x1, cube(x1), integral);
    }

    starting_time = omp_get_wtime() - starting_time;
    printf("\tExecution time %lf. \tCalculated integral = %.15lf\n\n\n", starting_time, integral);

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

    double starting_time = omp_get_wtime();
    int i;
    double integral = 0.0;

#pragma omp parallel for default(none) firstprivate(N, a, dx_adjust) reduction(+:integral)
    for (i = 0; i < N; i++)
    {
        double x1 = a + i * dx_adjust;
        integral += 0.5 * dx_adjust * (cube(x1) + cube(x1 + dx_adjust));

        //printf("i %d, x1 %lf, cube(x1) %lf, integral = %.15lf\n",
        //     i, x1, cube(x1), integral);
    }

    starting_time = omp_get_wtime() - starting_time;
    printf("%lf\n", starting_time);
}