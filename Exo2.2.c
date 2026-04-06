#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid1, pid2;

    pid1 = fork();  // Création du fils 1
    if (pid1 < 0)
    {
        perror("Erreur de création de fork");
        return (1);
    } 
    else if (pid1 == 0)
    {
        // Processus fils 1
        printf("Je suis le fils 1\n");
        return (0);
    }

    // Création du fils 2
    pid2 = fork();
    if (pid2 < 0) 
    {
        
        perror("Erreur de création de fork");
        return (1);
    } 
    else if (pid2 == 0)
    {
        // Processus fils 2
        printf("Je suis le fils 2\n");
        return (0);
    }

    // Processus père
    printf("Je suis le père\n");

    // Attendre que les deux fils se terminent
    wait(NULL);
    wait(NULL);

    return (0);
}