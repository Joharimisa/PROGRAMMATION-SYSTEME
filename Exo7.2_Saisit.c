#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) 
{
    if (argc != 2)
    {
        printf("Usage: %s <PID_affiche>\n", argv[0]);
        return (1);
    }

    int pid = atoi(argv[1]);

    int x;
    printf("Entrez un entier : ");
    scanf("%d", &x);

    FILE *f = fopen("/tmp/entier.txt", "w");
    if (!f) 
    {
        perror("fopen");
        return 1;
    }

    fprintf(f, "%d", x);
    fclose(f);

    // envoyer signal
    kill(pid, SIGUSR1);

    printf("Entier envoyé au processus %d\n", pid);

    return (0);
}