#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "czytelnia.h"

int my_read_lock_lock(Library *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);

    if (czytelnia_p->writers > 0 || czytelnia_p->writersWaiting > 0) {
        czytelnia_p->readersWaiting++;
        pthread_cond_wait(&czytelnia_p->condReader, &czytelnia_p->mutex);
        czytelnia_p->readersWaiting--;
    }
    czytelnia_p->readers++;
    pthread_cond_signal(&czytelnia_p->condReader);

    pthread_mutex_unlock(&czytelnia_p->mutex);
}

int my_read_lock_unlock(Library *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);

    czytelnia_p->readers--;
    if (czytelnia_p->readers == 0) {
        pthread_cond_signal(&czytelnia_p->condWriter);
    }

    pthread_mutex_unlock(&czytelnia_p->mutex);
}

int my_write_lock_lock(Library *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);

    if (czytelnia_p->writers > 0 || czytelnia_p->writersWaiting > 0) {
        czytelnia_p->writersWaiting++;
        pthread_cond_wait(&czytelnia_p->condWriter, &czytelnia_p->mutex);
        czytelnia_p->writersWaiting--;

    }
    czytelnia_p->writers++;

    pthread_mutex_unlock(&czytelnia_p->mutex);
}

int my_write_lock_unlock(Library *czytelnia_p) {
    pthread_mutex_lock(&czytelnia_p->mutex);

    czytelnia_p->writers--;
    printf("WRITERS INSIDE: %d\n\n", czytelnia_p->writers);

    if (czytelnia_p->readersWaiting > 0) {
        pthread_cond_signal(&czytelnia_p->condReader);
    } else {
        pthread_cond_signal(&czytelnia_p->condWriter);
    }

    pthread_mutex_unlock(&czytelnia_p->mutex);
}

void initiate(Library *czytelnia_p) {
    czytelnia_p->mutex = PTHREAD_MUTEX_INITIALIZER;
    czytelnia_p->writers = 0;
    czytelnia_p->readers = 0;

    pthread_cond_init(&czytelnia_p->condWriter, NULL);
    pthread_cond_init(&czytelnia_p->condReader, NULL);

    czytelnia_p->writersWaiting = 0;
    czytelnia_p->readersWaiting = 0;
}

void reading(Library *czytelnia_p) {
    // wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
    printf("READERS INSIDE: %d\n", czytelnia_p->readers);
    printf("WRITERS INSIDE: %d\n", czytelnia_p->writers);

    // sprawdzenie warunku poprawności i ewentualny exit
    if (czytelnia_p->writers > 1 || (czytelnia_p->writers == 1 && czytelnia_p->readers > 0) ||
        czytelnia_p->writers < 0 || czytelnia_p->readers < 0) {
        printf("An Error has occurred");
        exit(0);
    }

    usleep(rand() % 300000);
}

void writing(Library *czytelnia_p) {
    // wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
    printf("READERS INSIDE: %d\n", czytelnia_p->readers);
    printf("WRITERS INSIDE: %d\n", czytelnia_p->writers);

    // sprawdzenie warunku poprawności i ewentualny exit
    if (czytelnia_p->writers > 1 || (czytelnia_p->writers == 1 && czytelnia_p->readers > 0) ||
        czytelnia_p->writers < 0 || czytelnia_p->readers < 0) {
        printf("An Error has occurred");
        exit(0);
    }

    usleep(rand() % 300000);
}