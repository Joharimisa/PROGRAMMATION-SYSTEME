#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 100

double tableau[N];
pthread_mutex_t mutex;

void* calcul_moyenne(void* arg)
{
    int i;
    while (1) 
    {
        pthread_mutex_lock(&mutex);
        // appliquer moyenne pour les indices 1 à 98
        for (i = 1; i < N-1; i++) 
        {
            tableau[i] = (tableau[i-1] + tableau[i] + tableau[i+1]) / 3.0;
        }
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 1); // 1 à 3 secondes
    }
    return (NULL);
}

void* afficher_tableau(void* arg) 
{
    int i;
    while (1) 
    {
        sleep(4);
        pthread_mutex_lock(&mutex);
        for (i = 0; i < N; i++) 
        {
            printf("%.2f ", tableau[i]);
        }
        printf("\n");
        pthread_mutex_unlock(&mutex);
    }
    return (NULL);
}

int main()
{
    srand(time(NULL));

    // initialisation tableau
    tableau[0] = 0;
    tableau[N-1] = 0;
    int i;
    for (i = 1; i < N-1; i++)
    {
        tableau[i] = (rand() % 10000) / 100.0; // valeurs 0.00 à 99.99
    }

    pthread_mutex_init(&mutex, NULL);

    pthread_t th_calc, th_aff;
    pthread_create(&th_calc, NULL, calcul_moyenne, NULL);
    pthread_create(&th_aff, NULL, afficher_tableau, NULL);

    pthread_join(th_calc, NULL);
    pthread_join(th_aff, NULL);

    pthread_mutex_destroy(&mutex);
    return (0);
}