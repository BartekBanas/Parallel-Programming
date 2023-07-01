#ifndef _czytelnia_
#define _czytelnia_

typedef struct {
    int readers;
    int writers;

    int readersWaiting;
    int writersWaiting;

    int writersLimit;

    pthread_mutex_t mutex;

    pthread_cond_t condWriter, condReader;

} Library;

void initiate(Library *czytelnia_p);

void reading(Library *czytelnia_p);

void writing(Library *czytelnia_p);

int my_read_lock_lock(Library *czytelnia_p);

int my_read_lock_unlock(Library *czytelnia_p);

int my_write_lock_lock(Library *czytelnia_p);

int my_write_lock_unlock(Library *czytelnia_p);

#endif