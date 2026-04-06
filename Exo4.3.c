#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() 
{
    int fd[2];
    if (pipe(fd) == -1)
    {
         perror("pipe");
          return 1;
    }

    pid_t pid = fork();
    if (pid < 0) 
    { 
        perror("fork");
        return 1; 
    }

    if (pid == 0) 
    {
    
        // --------- Fils ---------
        // Fermer l'extrémité écriture du fils
        close(fd[1]);

        // Préparer les arguments pour execv
        char fd_str[10];
        snprintf(fd_str, sizeof(fd_str), "%d", fd[0]);

        char *args[] = {"./fils", fd_str, NULL};
        execv(args[0], args);

        // Si execv échoue
        perror("execv");
        exit(1);
    } 
    else
    {
        // --------- Père ---------
        close(fd[0]); // fermer lecture

        char message[] = "Bonjour du père !";
        write(fd[1], message, strlen(message)+1); // +1 pour '\0'
        close(fd[1]);
    }

    return (0);
}