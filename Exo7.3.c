#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define N 5

pid_t fils[N];

// Handler pour les fils
void handler_fils(int sig) 
{
    printf("Fils %d : je suis tué\n", getpid());
    exit(0);
}

// Handler pour le père
void handler_pere(int sig)
{
    printf("Père : je suis tué\n");
    exit(0);
}

int main()
{
    signal(SIGTERM, handler_pere);
    int i;
    // Création des fils
    for (i = 0; i < N; i++)
    {
        pid_t pid = fork();

        if (pid == 0) 
        {
            // ----- FILS -----
            signal(SIGTERM, handler_fils);

            while (1) 
            {
                printf("Fils %d actif\n", getpid());
                sleep(2);
            }
        } 
        else
        {
            fils[i] = pid;
        }
    }

    // ----- PÈRE -----
    char choix;
    int index;

    while (1) 
    {
        printf("\nMenu:\n");
        printf("s: suspendre un fils\n");
        printf("r: réveiller un fils\n");
        printf("k: tuer un fils\n");
        printf("Choix: ");
        scanf(" %c", &choix);

        printf("Index du fils (0 à 4): ");
        scanf("%d", &index);

        if (index < 0 || index >= N) 
        {
            printf("Index invalide\n");
            continue;
        }

        switch (choix) 
        {
            case 's':
                kill(fils[index], SIGSTOP);
                printf("Fils %d suspendu\n", fils[index]);
                break;

            case 'r':
                kill(fils[index], SIGCONT);
                printf("Fils %d réveillé\n", fils[index]);
                break;

            case 'k':
                kill(fils[index], SIGTERM);
                printf("Signal de terminaison envoyé au fils %d\n", fils[index]);
                break;

            default:
                printf("Choix invalide\n");
        }
    }

    return (0);
}