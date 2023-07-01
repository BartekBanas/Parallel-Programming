#include <pthread.h>

#define THREADS_AMOUNT 4

static pthread_mutex_t lock;
static pthread_cond_t bariera_cond;
int amountOfThreads;
int awaitingThreads;

void barrierInitiation(int l) //zwiazanie mutexu i zmiennej warunku z liczba watkow
{
    pthread_mutex_init(&lock, NULL);    //tworzenie mutexu
    pthread_cond_init(&bariera_cond, NULL); //tworzenie zmiennej warunku
    amountOfThreads = l;
    awaitingThreads = 0;
}

void bariera() {
    pthread_mutex_lock(&lock);
    awaitingThreads++;

    if (awaitingThreads < amountOfThreads) {
        pthread_cond_wait(&bariera_cond, &lock);
    } else {
        awaitingThreads = 0;
        pthread_cond_broadcast(&bariera_cond);
    }
    pthread_mutex_unlock(&lock);
}