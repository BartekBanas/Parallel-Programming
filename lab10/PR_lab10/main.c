#include<stdio.h>
#include<omp.h>

#define THREAD_AMOUNT 10

int main() {

    static int f_threadprivate;

#ifdef   _OPENMP
    printf("\nKompilator rozpoznaje dyrektywy OpenMP\n");
#endif

    int a_shared = 1;
    int b_private = 2;
    int c_firstprivate = 3;
    int e_atomic = 5;

    omp_set_num_threads(THREAD_AMOUNT);

    printf("przed wejsciem do obszaru rownoleglego -  nr_threads %d, thread ID %d\n",
           omp_get_num_threads(), omp_get_thread_num());
    printf("\ta_shared \t= %d\n", a_shared);
    printf("\tb_private \t= %d\n", b_private);
    printf("\tc_firstprivate \t= %d\n", c_firstprivate);
    printf("\te_atomic \t= %d\n", e_atomic);


    printf("\nTestowanie threadprivate:\n");
#pragma omp threadprivate(f_threadprivate)
#pragma omp parallel default(none)
    {
        f_threadprivate = omp_get_thread_num();

        printf("My ID %d,\tf_threadprivate = %d\n", omp_get_thread_num(), f_threadprivate);
    }


#pragma omp parallel default(none) shared(a_shared, e_atomic) private(b_private) firstprivate(c_firstprivate )
    {
        int i;
        int d_local_private;


        d_local_private = a_shared + c_firstprivate;


#pragma omp barrier
#pragma omp critical (a_shared)
        for (i = 0; i < 10; i++) {
            a_shared++;
        }

        for (i = 0; i < 10; i++) {
            c_firstprivate += omp_get_thread_num();
        }

        for (i = 0; i < 10; i++) {
#pragma omp atomic
            e_atomic += omp_get_thread_num();
        }

//#pragma omp barrier
#pragma omp critical (printing)
        {
            printf("\nw obszarze rownoleglym: aktualna liczba watkow %d, moj ID %d\n",
                   omp_get_num_threads(), omp_get_thread_num());

            printf("\ta_shared \t= %d\n", a_shared);
            printf("\tb_private \t= %d\n", b_private);
            printf("\tc_firstprivate \t= %d\n", c_firstprivate);
            printf("\td_local_private = %d\n", d_local_private);
            printf("\te_atomic \t= %d\n", e_atomic);

        }
    }

    printf("po zakonczeniu obszaru rownoleglego:\n");
    printf("\ta_shared \t= %d\n", a_shared);
    printf("\tb_private \t= %d\n", b_private);
    printf("\tc_firstprivate \t= %d\n", c_firstprivate);
    printf("\te_atomic \t= %d\n", e_atomic);
}
