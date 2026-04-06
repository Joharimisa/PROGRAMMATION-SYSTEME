#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int compteur = 0;      // compteur global
pthread_mutex_t mutex;  // mutex pour protéger compteur
int limite;             // limite passée en argument

void* incrementer(void* arg) 
{
    while (1) 
    {
        sleep(rand() % 5 + 1); // 1 à 5 secondes

        pthread_mutex_lock(&mutex);
        if (compteur >= limite) 
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        compteur++;
        pthread_mutex_unlock(&mutex);
    }
    return (NULL);
}

void* afficher(void* arg) 
{
    while (1)
    {
        sleep(2);
        pthread_mutex_lock(&mutex);
        if (compteur >= limite) 
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("Compteur = %d\n", compteur);
        pthread_mutex_unlock(&mutex);
    }
    return (NULL);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <limite>\n", argv[0]);
        return (1);
    }

    limite = atoi(argv[1]);
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);

    pthread_t th_inc, th_aff;

    pthread_create(&th_inc, NULL, incrementer, NULL);
    pthread_create(&th_aff, NULL, afficher, NULL);

    pthread_join(th_inc, NULL);
    pthread_join(th_aff, NULL);

    printf("Compteur final = %d\n", compteur);
    pthread_mutex_destroy(&mutex);

    return (0);
}