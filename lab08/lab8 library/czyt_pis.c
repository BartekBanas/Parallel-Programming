#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "czytelnia.h"

_Noreturn void *readerFunction(void *);

_Noreturn void *writerFunction(void *);

int main()
{
    int i;
    pthread_t pisarze[5], czytelnicy[10];
    Library library;

    initiate(&library);

    for (i = 0; i < 5; i++) {
        pthread_create(&pisarze[i], NULL, writerFunction, (void *) &library);
    }
    for (i = 0; i < 10; i++) {
        pthread_create(&czytelnicy[i], NULL, readerFunction, (void *) &library);
    }
    for (i = 0; i < 5; i++) {
        pthread_join(pisarze[i], NULL);
    }
    for (i = 0; i < 10; i++) {
        pthread_join(czytelnicy[i], NULL);
    }
}

_Noreturn void *readerFunction(void *arg)
{
    Library *libraryPointer = (Library *) arg;

    for (;;)
    {
        usleep(rand() % 1000000);
        printf("reader %llu - at the door\n", pthread_self());

        my_read_lock_lock(libraryPointer);

        printf("reader %llu - entering\n", pthread_self());

        reading(libraryPointer);


        printf("reader %llu - entering\n", pthread_self());

        my_read_lock_unlock(libraryPointer);

        printf("reader %llu - left\n", pthread_self());
    }
}

_Noreturn void *writerFunction(void *arg) {

    Library *czytelnia_p = (Library *) arg;

    for (;;) {

        usleep(rand() % 3000000);
        printf("writer %llu - at the door\n", pthread_self());

        my_write_lock_lock(czytelnia_p);

        printf("writer %llu - entering\n", pthread_self());

        writing(czytelnia_p);

        printf("writer %llu - entering\n", pthread_self());

        my_write_lock_unlock(czytelnia_p);

        printf("writer %llu - left\n", pthread_self());
    }
}