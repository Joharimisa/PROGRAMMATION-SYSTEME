#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t sem_T1, sem_T2;

void* T1(void* arg)
{
    int i;
    for (i = 0; i < 10; i++) 
    {
        sleep(2); // traitement T1

        printf("T1 terminé itération %d\n", i);

        sem_post(&sem_T2); // signaler T2
        sem_wait(&sem_T1); // attendre T2

        printf("T1 reprend\n");
    }
    return (NULL);
}

void* T2(void* arg)
{
    int i;
    for (i = 0; i < 10; i++) 
    {
        sleep(rand() % 6 + 4); // 4 à 9 secondes

        printf("T2 terminé itération %d\n", i);

        sem_post(&sem_T1); // signaler T1
        sem_wait(&sem_T2); // attendre T1

        printf("T2 reprend\n");
    }
    return (NULL);
}

int main()
{
    srand(time(NULL));

    pthread_t t1, t2;

    sem_init(&sem_T1, 0, 0);
    sem_init(&sem_T2, 0, 0);

    pthread_create(&t1, NULL, T1, NULL);
    pthread_create(&t2, NULL, T2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem_T1);
    sem_destroy(&sem_T2);

    return (0);
}