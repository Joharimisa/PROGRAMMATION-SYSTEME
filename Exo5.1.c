#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 5

// Fonction du thread
void* afficher_tableau(void* arg) 
{
    int* tab = (int*) arg;
    printf("Thread : tableau = ");
    for (int i = 0; i < SIZE; i++) 
    {
        printf("%d ", tab[i]);
    }
    printf("\n");
    return (NULL);
}

int main() 
{
    pthread_t tid;
    int tab[SIZE] = {1, 2, 3, 4, 5};

    // Création du thread
    if (pthread_create(&tid, NULL, afficher_tableau, tab) != 0) 
    {
        perror("pthread_create");
        return (1);
    }

    // Attendre la fin du thread
    pthread_join(tid, NULL);

    printf("Thread terminé\n");
    return (0);
}