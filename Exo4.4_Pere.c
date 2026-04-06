#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() 
{
    int fd[2];
    if (pipe(fd) == -1)
    { 
        perror("pipe");
         return (1);
    }

    pid_t pid = fork();
    if (pid < 0) 
    { 
        perror("fork"); 
        return 1; 
    }

    if (pid == 0) 
    {
        // -------- Fils --------
        close(fd[1]); // fermer écriture côté fils

        // Préparer les variables d'environnement
        char env_fd[50];
        snprintf(env_fd, sizeof(env_fd), "FD_TUBE=%d", fd[0]);
        char *envp[] = {env_fd, NULL};

        execvpe("./fils_env", NULL, envp);  // execvpe pour passer env
        perror("execvpe"); 
        exit(1);
    } 
    else
    {
        // -------- Père --------
        close(fd[0]); // fermer lecture côté père
        char message[] = "Bonjour du père via env!";
        write(fd[1], message, strlen(message)+1);
        close(fd[1]);
    }

    return (0);
}