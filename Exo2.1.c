#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() 
{
    pid_t pid = fork();  // Crée un processus fils

    if (pid < 0)
    {
        // Erreur lors de la création du fils
        perror("Erreur de création");
        return (1);
    } else if (pid == 0) 
    {
        // Processus fils
        printf("Je suis le fils\n");
    }
    else 
    {
        // Processus père
        printf("Je suis le père\n");
    }

    return (0);
}