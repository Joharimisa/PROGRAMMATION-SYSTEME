#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define SIZE 10

char buffer[SIZE];
int top = -1;

sem_t vide, plein;
pthread_mutex_t mutex;

void* producteur(void* arg) 
{
    int id = *(int*)arg;

    while (1) 
    {
        sleep(rand() % 3 + 1);

        char item = 'A' + rand() % 26;

        sem_wait(&vide); // attendre place libre

        pthread_mutex_lock(&mutex);

        top++;
        buffer[top] = item;
        printf("Producteur %d produit %c (top=%d)\n", id, item, top);

        pthread_mutex_unlock(&mutex);

        sem_post(&plein); // signaler élément dispo
    }
    return (NULL);
}

void* consommateur(void* arg)
{
    int id = *(int*)arg;

    while (1) 
    {
        sleep(rand() % 3 + 1);

        sem_wait(&plein); // attendre élément

        pthread_mutex_lock(&mutex);

        char item = buffer[top];
        top--;
        printf("Consommateur %d consomme %c (top=%d)\n", id, item, top);

        pthread_mutex_unlock(&mutex);

        sem_post(&vide); // libérer place
    }
    return (NULL);
}

int main() 
{
    int N, M,i;

    printf("Nombre de producteurs : ");
    scanf("%d", &N);

    printf("Nombre de consommateurs : ");
    scanf("%d", &M);

    srand(time(NULL));

    pthread_t prod[N], cons[M];
    int id_prod[N], id_cons[M];

    sem_init(&vide, 0, SIZE);
    sem_init(&plein, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // créer producteurs
    for (i = 0; i < N; i++) 
    {
        id_prod[i] = i;
        pthread_create(&prod[i], NULL, producteur, &id_prod[i]);
    }

    // créer consommateurs
    for (i = 0; i < M; i++) 
    {
        id_cons[i] = i;
        pthread_create(&cons[i], NULL, consommateur, &id_cons[i]);
    }

    for (i = 0; i < N; i++)
    {
        pthread_join(prod[i], NULL);
    }
    for (i = 0; i < M; i++)
    {
        pthread_join(cons[i], NULL);
    
    }
    sem_destroy(&vide);
    sem_destroy(&plein);
    pthread_mutex_destroy(&mutex);

    return (0);
}