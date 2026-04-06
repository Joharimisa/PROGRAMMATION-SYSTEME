#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct
{
    int* tab;  // tableau
    int n;     // nombre d'éléments
    int x;     // entier à chercher
} TypeTableau;

// Thread 1 : initialisation du tableau avec valeurs aléatoires
void* generer_tableau(void* arg)
{
    int i;
    TypeTableau* t = (TypeTableau*)arg;
    t->tab = malloc(t->n * sizeof(int));
    if (!t->tab) 
    {
        return (NULL);
    }    

    for (i = 0; i < t->n; i++)
    {
        t->tab[i] = rand() % 100;
    }

    printf("Thread de génération : tableau généré.\n");
    return (NULL);
}

// Thread 2 : recherche x dans le tableau
void* chercher_x(void* arg)
{
    int i;
    TypeTableau* t = (TypeTableau*)arg;
    int found = 0;
    for (i = 0; i < t->n; i++) 
    {
        if (t->tab[i] == t->x) 
        {
            found = 1;
            break;
        }
    }
    printf("Thread recherche : %d %s dans le tableau.\n", t->x, found ? "est" : "n'est pas");
    return (NULL);
}

int main() 
{
    srand(time(NULL));

    TypeTableau t;
    printf("Nombre d'éléments du tableau : ");
    scanf("%d", &t.n);

    pthread_t tid_gen, tid_search;

    // Créer le thread de génération
    pthread_create(&tid_gen, NULL, generer_tableau, &t);

    //le thread principal lit x
    printf("Entier à chercher x : ");
    scanf("%d", &t.x);

    // Attendre que le tableau soit généré
    pthread_join(tid_gen, NULL);

    // Créer thread pour chercher x
    pthread_create(&tid_search, NULL, chercher_x, &t);
    pthread_join(tid_search, NULL);

    free(t.tab);

    return (0);
}