#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

pthread_mutex_t mutexRead = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexWrite = PTHREAD_MUTEX_INITIALIZER;

static int readers = 0;
static int writers = 0;
static int writersLimit = 1;

/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t *czytelnia_p) {
    pthread_mutex_trylock(&mutexRead);
    if(readers < 0)
    {
        printf("Error - negative amount of readers!\n\n");
        exit(1);
    }

    readers++;

    pthread_mutex_unlock(&mutexRead);
}


int my_read_lock_unlock(czytelnia_t *czytelnia_p) {
    pthread_mutex_trylock(&mutexRead);
    if(readers < 0)
    {
        printf("Error - negative amount of readers!\n\n");
        exit(1);
    }

    readers--;

    pthread_mutex_unlock(&mutexRead);
}


int my_write_lock_lock(czytelnia_t *czytelnia_p) {

    if(writers < 0)
    {

        printf("Error - negative amount of writers!\n\n");
        exit(1);
    }

    while (1) {
        pthread_mutex_trylock(&mutexWrite);
        if(writers < writersLimit) {
            writers++;
            printf("WRITERS INSIDE: %d\n\n", writers);
            break;
        }

        pthread_mutex_unlock(&mutexWrite);
        usleep(100);
    }

}


int my_write_lock_unlock(czytelnia_t *czytelnia_p) {
    pthread_mutex_trylock(&mutexWrite);
    if(writers < 0)
    {
        printf("Error - negative amount of writers!\n\n");
        exit(1);
    }
    if(writers>0)
    writers--;
    printf("WRITERS INSIDE: %d\n\n", writers);

    pthread_mutex_unlock(&mutexWrite);
}

void inicjuj(czytelnia_t *czytelnia_p) {

}

void czytam(czytelnia_t *czytelnia_p) {

// wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy

// sprawdzenie warunku poprawności i ewentualny exit
// warunek: if( l_p>1 || (l_p==1 && l_c>0) || l_p<0 || l_c<0 ) { printf(...); exit(0); }

    usleep(rand() % 300000);
}

void pisze(czytelnia_t *czytelnia_p) {

// wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy

// sprawdzenie warunku poprawności i ewentualny exit
// warunek: if( l_p>1 || (l_p==1 && l_c>0) || l_p<0 || l_c<0 ) { printf(...); exit(0); }

    usleep(rand() % 300000);
}