#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
    int* tab;
    int n;
} Tableau;

int cancel_flag = 0; // global pour interruption

// Génération de tableau aléatoire
void* generer_tableau(void* arg) 
{
    int i;
    Tableau* t = (Tableau*) arg;
    t->tab = malloc(t->n * sizeof(int));
    if (!t->tab) 
    {
        return (NULL);
    }    
    for (i = 0; i < t->n; i++) 
    {
        t->tab[i] = rand() % 100;
        if (cancel_flag)
        {
            return (NULL); // interruption
        }    
    }
    return (NULL);
}

// Vérification si T1 est inclus dans T2
void* verifier_inclusion(void* arg) 
{
    Tableau* tab = (Tableau*) arg;
    Tableau* T1 = &tab[0];
    Tableau* T2 = &tab[1];
    int i,j;
    int included = 1;
    for (i = 0; i < T1->n; i++) 
    {
        int found = 0;
        for (j = 0; j < T2->n; j++) 
        {
            if (T1->tab[i] == T2->tab[j]) 
            {
                found = 1;
                break;
            }
        }
        if (!found) 
        {
            included = 0;
            break;
        }
        if (cancel_flag) 
        {
            return (NULL); // interruption
    }
    printf("T1 %s T2\n", included ? "est inclus dans" : "n'est pas inclus dans");
    return (NULL);
}

// Thread pour surveiller entrée utilisateur
void* surveiller_entrée(void* arg) 
{
    char c;
    printf("Appuyez sur 'A' pour annuler...\n");
    c = getchar();
    if (c == 'A' || c == 'a') 
    {
        cancel_flag = 1;
        printf("Programme annulé par l'utilisateur !\n");
        exit(0);
    }
    return (NULL);
}

int main() 
{
    srand(time(NULL));

    Tableau T1, T2;
    T1.n = 5;  // plus petit
    T2.n = 10; // plus grand

    pthread_t th1, th2, th_incl, th_cancel;

    pthread_create(&th_cancel, NULL, surveiller_entrée, NULL);
    pthread_create(&th1, NULL, generer_tableau, &T1);
    pthread_create(&th2, NULL, generer_tableau, &T2);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    // Vérifier inclusion
    Tableau tab_arr[2] = {T1, T2};
    pthread_create(&th_incl, NULL, verifier_inclusion, tab_arr);
    pthread_join(th_incl, NULL);

    free(T1.tab);
    free(T2.tab);

    return (0);
}