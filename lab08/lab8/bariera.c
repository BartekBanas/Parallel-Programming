#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#define LICZBA_W 4

static pthread_mutex_t lock;
static pthread_cond_t bariera_cond;
int liczba_w;
int w_czekajace; //liczy liczbe watkow  1a

void bariera_init(int l) //zwiazanie mutexu i zmiennej warunku z liczba watkow
{
    pthread_mutex_init(&lock, NULL);    //tworzenie mutexu
    pthread_cond_init(&bariera_cond, NULL); //tworzenie zmiennej warunku
    liczba_w = l;
    w_czekajace = 0;
}

void bariera() {
    pthread_mutex_lock(&lock);
    w_czekajace++;
    if (w_czekajace < liczba_w) {
        pthread_cond_wait(&bariera_cond, &lock);
    } else {
        w_czekajace = 0;
        pthread_cond_broadcast(&bariera_cond);
    }
    pthread_mutex_unlock(&lock);
}
