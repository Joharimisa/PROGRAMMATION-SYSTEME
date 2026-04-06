#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define SIZE 15

char database[SIZE];
int readcount = 0;

sem_t mutex; // protège readcount
sem_t wrt;   // accès à la base

void* lecteur(void* arg) 
{
    int id = *(int*)arg;
    int i;

    while (1) 
    {
        sleep(rand() % 3 + 1);

        sem_wait(&mutex);
        readcount++;
        if (readcount == 1)
        {
            sem_wait(&wrt); // premier lecteur bloque écrivain
        }    
        sem_post(&mutex);

        // --- lecture ---
        i = rand() % SIZE;
        printf("Lecteur %d lit database[%d] = %d\n", id, i, database[i]);

        sem_wait(&mutex);
        readcount--;
        if (readcount == 0)
        {
            sem_post(&wrt); // dernier lecteur libère
        }    
        sem_post(&mutex);
    }
    return (NULL);
}

void* redacteur(void* arg)
{
    int id = *(int*)arg;
    int i;
    while (1) 
    {
        sleep(rand() % 10 + 1);

        sem_wait(&wrt);

        // --- écriture ---
        i = rand() % SIZE;
        database[i] = rand() % 256;
        printf("Rédacteur %d écrit database[%d] = %d\n", id, i, database[i]);

        sem_post(&wrt);
    }
    return (NULL);
}

int main()
{
    int N, M,i;

    printf("Nombre de lecteurs : ");
    scanf("%d", &N);

    printf("Nombre de rédacteurs : ");
    scanf("%d", &M);

    srand(time(NULL));

    pthread_t lecteurs[N], redacteurs[M];
    int idL[N], idR[M];

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // init base
    for (i = 0; i < SIZE; i++)
    {
        database[i] = 0;
    }
    // créer lecteurs
    for (i = 0; i < N; i++) 
    {
        idL[i] = i;
        pthread_create(&lecteurs[i], NULL, lecteur, &idL[i]);
    }

    // créer rédacteurs
    for (i = 0; i < M; i++)
    {
        idR[i] = i;
        pthread_create(&redacteurs[i], NULL, redacteur, &idR[i]);
    }

    for (i = 0; i < N; i++)
    {
        pthread_join(lecteurs[i], NULL);
    }    

    for (i = 0; i < M; i++)
    {
        pthread_join(redacteurs[i], NULL);
    }    

    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return (0);
}