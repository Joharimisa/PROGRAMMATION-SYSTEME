#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) 
{
    FILE *f = fopen("/tmp/entier.txt", "r");
    if (!f) 
    {
        perror("fopen");
        exit(1);
    }

    int x;
    fscanf(f, "%d", &x);
    fclose(f);

    printf("Valeur reçue : %d\n", x);
}

int main() 
{
    printf("PID du programme affiche : %d\n", getpid());

    signal(SIGUSR1, handler);

    while (1) 
    {
        sleep(1); // attente passive
    }

    return (0);
}