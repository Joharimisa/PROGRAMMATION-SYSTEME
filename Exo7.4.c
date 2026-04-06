#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

int *tab = NULL;
int n;
jmp_buf env;

// Handler pour SIGSEGV
void segfault_handler(int sig)
{
    printf("Erreur de segmentation ! Indice invalide. Ressaisir l'indice.\n");
    longjmp(env, 1);
}

int main() 
{
    signal(SIGSEGV, segfault_handler);
    int i;
    printf("Entrez le nombre d'elements n : ");
    scanf("%d", &n);

    tab = (int*) malloc(n * sizeof(int));
    if (!tab) 
    {
        perror("malloc");
        return (1);
    }

    // Saisie du tableau
    for (i = 0; i < n; i++) 
    {
        printf("tab[%d] = ", i);
        scanf("%d", &tab[i]);
    }

    while (1) 
    {
        if (setjmp(env) == 0 || 1)
        { // point de retour après SIGSEGV
            printf("Entrez l'indice i : ");
            scanf("%d", &i);

            // Tentative d'accès
            printf("tab[%d] = %d\n", i, tab[i]);
            break; // succès
        }
    }

    free(tab);
    return (0);
}