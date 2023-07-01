#include<stdio.h>
#include<pthread.h>
#include"bariera.h"

#define THREADS_AMOUNT 44

pthread_t threads[THREADS_AMOUNT];

void *whatever(void *arg);

int main(int argc, char *argv[]) {
    int i, indexes[THREADS_AMOUNT];
    for (i = 0; i < THREADS_AMOUNT; i++) indexes[i] = i;

    // use of pthread_once (once_control, init_routine) ?
    // int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
    // but init_routine has no arguments...
    barrierInitiation(THREADS_AMOUNT);

    for (i = 0; i < THREADS_AMOUNT; i++) {
        pthread_create(&threads[i], NULL, whatever, (void *) &indexes[i]);
    }

    for (i = 0; i < THREADS_AMOUNT; i++) pthread_join(threads[i], NULL);

    pthread_exit(NULL);
}

void *whatever(void *arg) {

    int i, id;

    id = *((int *) arg);

    printf("before barrier 1 - thread %d\n", id);

    barrier();

    printf("before barrier 2 - thread %d\n", id);

    barrier();

    printf("before barrier 3 - thread %d\n", id);

    barrier();

    printf("before barrier4 - thread %d\n", id);

    barrier();

    printf("before barrier 5 - thread %d\n", id);

    barrier();

    printf("after the last barrier - thread %d\n", id);

    pthread_exit((void *) 0);
}