#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// ==========================
// Variables globales
// ==========================
int compteur = 0;
double u;

// Mutex pour protéger les accès concurrents
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// ==========================
// f1(x) = (1/4)*(x-1)^2
// ==========================
double f1(double x)
{
    double res;

    res = (1.0 / 4.0) * (x - 1.0) * (x - 1.0);

    return res;
}

// ==========================
// f2(x) = (1/6)*(x-2)^2
// ==========================
double f2(double x)
{
    double res;

    res = (1.0 / 6.0) * (x - 2.0) * (x - 2.0);

    return res;
}

// ==========================
// Thread T1
// ==========================
void *T1(void *arg)
{
    int attente;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        compteur = compteur + 1;
        u = f1(u);

        printf("[T1] Iteration %d | u = %f\n", compteur, u);

        pthread_mutex_unlock(&mutex);

        attente = (rand() % 5) + 1;
        sleep(attente);
    }

    return NULL;
}

// ==========================
// Thread T2
// ==========================
void *T2(void *arg)
{
    int attente;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        compteur = compteur + 1;
        u = f2(u);

        printf("[T2] Iteration %d | u = %f\n", compteur, u);

        pthread_mutex_unlock(&mutex);

        attente = (rand() % 5) + 1;
        sleep(attente);
    }

    return NULL;
}

// ==========================
// Main
// ==========================
int main()
{
    pthread_t th1;
    pthread_t th2;

    srand(time(NULL));

    u = 1.0;

    pthread_create(&th1, NULL, T1, NULL);
    pthread_create(&th2, NULL, T2, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    return 0;
}