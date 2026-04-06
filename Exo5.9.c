#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

char buffer; // variable globale (1 octet)

sem_t vide;
sem_t plein;

void* emetteur(void* arg) 
{
    while (1) 
    {
        sleep(rand() % 3 + 1);

        sem_wait(&vide); // attendre que buffer soit libre

        buffer = 'A' + rand() % 26;
        printf("Émetteur envoie : %c\n", buffer);

        sem_post(&plein); // signaler qu'il y a une donnée
    }
    return (NULL);
}

void* recepteur(void* arg) 
{
    while (1) 
    {
        sleep(rand() % 3 + 1);

        sem_wait(&plein); // attendre qu'il y ait une donnée

        printf("Récepteur lit : %c\n", buffer);

        sem_post(&vide); // libérer le buffer
    }
    return (NULL);
}

int main() 
{
    srand(time(NULL));

    pthread_t t1, t2;

    sem_init(&vide, 0, 1);
    sem_init(&plein, 0, 0);

    pthread_create(&t1, NULL, emetteur, NULL);
    pthread_create(&t2, NULL, recepteur, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&vide);
    sem_destroy(&plein);

    return (0);
}