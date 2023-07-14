#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define LEFT_TO_DRINK 3

void *customer_threads(void *pointer);

pthread_mutex_t mutex;

int amount_of_customers, number_of_pints, number_of_pints;
int bug = 0;

int main(void)
{
    pthread_t *clients;
    int *client_ids;

    pthread_mutex_init(&mutex, NULL);

    int i;

    printf("\nNumber of Clients: ");
    scanf("%d", &amount_of_customers);

    printf("\nNumber of pints: ");
    scanf("%d", &number_of_pints);
    int kufelki = number_of_pints;

    //printf("\nNumber of taps: "); scanf("%d", &number_of_pints);
    number_of_pints = 1000000000; // enough that there is no competition

    clients = (pthread_t *) malloc(amount_of_customers * sizeof(pthread_t));
    client_ids = (int *) malloc(amount_of_customers * sizeof(int));
    for (i = 0; i < amount_of_customers; i++) client_ids[i] = i;


    printf("\nPub opening(simple)!\n");
    printf("\nNumber of free pints %d\n", number_of_pints);

    for (i = 0; i < amount_of_customers; i++) {
        pthread_create(&clients[i], NULL, customer_threads, &client_ids[i]);
    }
    for (i = 0; i < amount_of_customers; i++) {
        pthread_join(clients[i], NULL);
    }
    printf("\nPub closing!\n");

    printf("\nNumber of clients: %d", amount_of_customers);
    printf("\nInitial number of pints: %d", kufelki);
    printf("\nNumber of pints: %d\n", number_of_pints);
    printf("\nThe number of errors when the number of pints reached a negative value: %d\n\n", bug);

    //pthread_mutex_destroy(&mutex);
}


void *customer_threads(void *pointer) {
    int id = *((int *) pointer);

    int i, j, success;
    int left_to_drink = LEFT_TO_DRINK;

    long int work_done = 0;

    printf("\nCustomer %d, I'm entering a pub\n", id);

    for (i = 0; i < left_to_drink; i++)
    {
        j = 0;

        do { success = 0;
            pthread_mutex_lock(&mutex);

            if (number_of_pints >= 1) {
                printf("\nCustomer %d, I choose a pint\n", id);
                number_of_pints--;

                if (number_of_pints < 0) { bug++; }

                success = 1;
                pthread_mutex_unlock(&mutex);
            }

            if (success == 0) {
                printf("\nCustomer %d, just waitin'\n", id);
                pthread_mutex_unlock(&mutex);
            }

        } while (success == 0);

        printf("\nCustomer %d, I'm pouring from the tap %d\n", id, j);

        number_of_pints--;
        usleep(30);
        number_of_pints++;

        printf("\nCustomer %d, I'm drinking\n", id);
        nanosleep((struct timespec[]) {{0, 50000000L}}, NULL);

        printf("\nCustomer %d, I'm putting down my pint\n", id);
        pthread_mutex_lock(&mutex);
        number_of_pints++;
        pthread_mutex_unlock(&mutex);
    }

    printf("\nCustomer %d, leaving the pub; the work done %ld\n", id, work_done);

    return (NULL);
}