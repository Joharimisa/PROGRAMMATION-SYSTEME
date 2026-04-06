#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 10

// Fonction du thread
void* generer_tableau(void* arg) 
{
    int* tab = malloc(SIZE * sizeof(int));
    if (!tab) return NULL;

    for (int i = 0; i < SIZE; i++) {
        tab[i] = rand() % 100; // 0 à 99
    }

    return (tab); // retour via void*
}

int main()
{
    pthread_t tid;
    srand(time(NULL));

    if (pthread_create(&tid, NULL, generer_tableau, NULL) != 0) 
    {
        perror("pthread_create");
        return (1);
    }

    int* resultat;
    pthread_join(tid, (void**)&resultat); // récupérer le tableau

    if (resultat)
    {
        printf("Tableau généré par le thread : ");
        for (int i = 0; i < SIZE; i++)
        {
            printf("%d ", resultat[i]);
        }
        printf("\n");
        free(resultat); // libération mémoire
    }

    return (0);
}