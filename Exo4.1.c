#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int fd1[2], fd2[2]; // fd1: fils->père, fd2: père->fils
    pipe(fd1);
    pipe(fd2);

    pid_t pid = fork();
    if (pid < 0)
    {
      perror("Erreur de création de fork");
       return (1);
    }

    if (pid == 0) 
    {
        // Fils
        close(fd1[0]); // fermer lecture fils->père
        close(fd2[1]); // fermer écriture père->fils

        char mot[100];
        printf("Fils : Entrez un mot : ");
        scanf("%99s", mot);

        write(fd1[1], mot, strlen(mot)+1);
        close(fd1[1]);

        // Lire résultat du père
        int result;
        read(fd2[0], &result, sizeof(int));
        close(fd2[0]);

        printf("Fils : Le mot %s dans le fichier.\n", result ? "est" : "n'est pas");
        exit(0);
    } 
    else
    {
        // Père
        close(fd1[1]); // fermer écriture fils->père
        close(fd2[0]); // fermer lecture père->fils

        char mot[100];
        read(fd1[0], mot, sizeof(mot));
        close(fd1[0]);

        FILE *f = fopen("fichier.txt", "r");
        if (!f) 
        {
            perror("fopen");
             return (1);
        }

        char word[100];
        int found = 0;
        while (fscanf(f, "%99s", word) == 1)
        {
            if (strcmp(word, mot) == 0) 
            { 
                found = 1; break;
            }
        }
        fclose(f);

        write(fd2[1], &found, sizeof(int));
        close(fd2[1]);
    }

    return (0);
}