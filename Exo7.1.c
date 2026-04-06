#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() 
{
    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        // ----- FILS -----
        while (1)
        {
            printf("Fils en cours de calcul...\n");
            sleep(1);
        }
    } 
    else 
    {
        // ----- PÈRE -----
        char c;

        while (1) 
        {
            printf("\nMenu : s (stop), r (resume), q (quit) : ");
            scanf(" %c", &c);

            if (c == 's') 
            {
                kill(pid, SIGSTOP);
                printf("Fils suspendu\n");
            } 
            else if (c == 'r') 
            {
                kill(pid, SIGCONT);
                printf("Fils repris\n");
            } 
            else if (c == 'q') 
            {
                kill(pid, SIGKILL);
                printf("Fils tué, fin du programme\n");
                break;
            }
        }
    }

    return (0);
}