#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define LEFT_TO_DRINK 3

void *watek_klient(void *arg);

pthread_mutex_t mutex;

int l_kl, l_kf, l_kr;
int bug = 0;

int main(void) {

    pthread_t *tab_klient;
    int *tab_klient_id;

    pthread_mutex_init(&mutex, NULL);

    int i;

    printf("\nLiczba klientow: ");
    scanf("%d", &l_kl);

    printf("\nLiczba kufli: ");
    scanf("%d", &l_kf);
    int kufelki = l_kf;

    //printf("\nLiczba kranow: "); scanf("%d", &l_kr);
    l_kr = 1000000000; // wystarczająco dużo, żeby nie było rywalizacji

    tab_klient = (pthread_t *) malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *) malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) tab_klient_id[i] = i;


    printf("\nOtwieramy pub (simple)!\n");
    printf("\nLiczba wolnych kufli %d\n", l_kf);

    for (i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }
    printf("\nZamykamy pub!\n");

    printf("\nLiczba klientow: %d", l_kl);
    printf("\nLiczba poczatkowa kufli: %d", kufelki);
    printf("\nLiczba kufli: %d\n", l_kf);
    printf("\nIlosc bledow, gdy ilosc kufli osiagnela wartosc ujemna: %d\n\n", bug);

    //pthread_mutex_destroy(&mutex, NULL);
}


void *watek_klient(void *arg_wsk) {
    int moj_id = *((int *) arg_wsk);

    int i, j, result, success = 0;
    int ile_musze_wypic = LEFT_TO_DRINK;

    long int wykonana_praca = 0;

    printf("\nKlient %d, wchodze do pubu\n", moj_id);

    for (i = 0; i < ile_musze_wypic; i++)
    {
        j = 0;

        do { success = 0;
            pthread_mutex_lock(&mutex);

            if (l_kf >= 1) {
                printf("\nKlient %d, wybieram kufel\n", moj_id);
                l_kf--;

                if (l_kf < 0) { bug++; }

                success = 1;
                pthread_mutex_unlock(&mutex);
            }

            if (success == 0) {
                printf("\nKlient %d, just waitin'\n", moj_id);
                pthread_mutex_unlock(&mutex);
            }

        } while (success == 0);

        printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j);

        l_kr--;
        usleep(30);
        l_kr++;

        printf("\nKlient %d, pije\n", moj_id);
        nanosleep((struct timespec[]) {{0, 50000000L}}, NULL);

        printf("\nKlient %d, odkladam kufel\n", moj_id);
        pthread_mutex_lock(&mutex);
        l_kf++;
        pthread_mutex_unlock(&mutex);
    }

    printf("\nKlient %d, wychodze z pubu; wykonana praca %ld\n",
           moj_id, wykonana_praca);

    //printf("\nLiczba kufli: %d", l_kf);

    return (NULL);
}