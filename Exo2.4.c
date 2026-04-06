#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int num_fils = 0;  // Pour stocker le numéro du fils

int main() 
{
    pid_t pid;
    int i;

    for (i = 1; i <= 5; i++) 
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            return (1);
        } 
        else if (pid == 0)
        {
            num_fils = i;  // Chaque fils reçoit son numéro
            printf("Je suis le fils %d\n", num_fils);
            break;  // Le fils ne continue pas la boucle
        }
        // Le père continue la boucle
    }

    // Le père attend tous les fils
    if (pid > 0) 
    {
        for (i = 0; i < 5; i++)
        {
            wait(NULL);
        }
        printf("Je suis le père\n");
    }

    return (0);
}