#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() 
{
    int i;
    pid_t pid;

    for (i = 1; i <= 5; i++) 
    {
        pid = fork();

        if (pid < 0)
        {
            perror("Erreur de création de fork");
            return (1);
        } else if (pid == 0) 
        {
            // Code du fils
            printf("Je suis le fils %d\n", i);
            break;  //Arrêter la boucle pour que le fils ne crée pas d'autres fils
        }
        // Le père continue la boucle pour créer les autres fils
    }

    // Le père attend tous les fils pour éviter les zombies
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