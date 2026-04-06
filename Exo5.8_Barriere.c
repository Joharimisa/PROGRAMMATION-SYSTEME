#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

int N;
int compteur = 0;

sem_t mutex;
sem_t barriere;

void* thread_func(void* arg)
{
    int id = *(int*)arg;
    int i,j;
    for (i = 0; i < 10; i++) 
    {
        sleep(rand() % 5 + 1); // 1 à 5 sec

        printf("Thread %d terminé itération %d\n", id, i);

        // section critique
        sem_wait(&mutex);
        compteur++;

        if (compteur == N) 
        {
            // dernier thread → libère tout le monde
            for (j = 0; j < N; j++) 
            {
                sem_post(&barriere);
            }
            compteur = 0; // reset pour prochaine itération
        }
        sem_post(&mutex);

        // attendre les autres
        sem_wait(&barriere);

        printf("Thread %d reprend\n", id);
    }

    return (NULL);
}

int main(int argc, char* argv[])
{
    int i;
    if (argc != 2) 
    {
        printf("Usage: %s N\n", argv[0]);
        return (1);
    }

    N = atoi(argv[1]);
    srand(time(NULL));

    pthread_t threads[N];
    int ids[N];

    sem_init(&mutex, 0, 1);
    sem_init(&barriere, 0, 0);

    for (i = 0; i < N; i++) 
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    for (i = 0; i < N; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&barriere);

    return (0);
}