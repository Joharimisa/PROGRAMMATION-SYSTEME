#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int pipe_fils_pere[2]; // fils -> père
    int pipe_pere_fils[2]; // père -> fils

    pipe(pipe_fils_pere);
    pipe(pipe_pere_fils);

    pid_t pid = fork();
    if (pid < 0)
    {
         perror("Erreur de création de fork"); 
         return (1); 
    }

    if (pid == 0) 
    {
        // --------- Fils ---------
        close(pipe_fils_pere[0]); // fermer lecture
        close(pipe_pere_fils[1]); // fermer écriture

        // Redirection stdin/stdout
        dup2(pipe_pere_fils[0], STDIN_FILENO);
        dup2(pipe_fils_pere[1], STDOUT_FILENO);

        // Lecture mot depuis stdin
        char mot[100];
        printf("Fils : Entrez un mot : ");
        fflush(stdout); // force affichage avant lecture
        scanf("%99s", mot);

        // Écriture vers stdout (redirigé vers le père)
        printf("%s\n", mot);
        fflush(stdout);

        exit(0);
    } 
    else
    {
        // --------- Père ---------
        close(pipe_fils_pere[1]); // fermer écriture
        close(pipe_pere_fils[0]); // fermer lecture

        char mot[100];
        read(pipe_fils_pere[0], mot, sizeof(mot));

        // Retirer \n si présent
        mot[strcspn(mot, "\n")] = 0;

        // Recherche dans le fichier
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
                found = 1; 
                break;
            }
        }
        fclose(f);

        // Envoyer résultat au fils via stdin (redirigé)
        char res[2];
        sprintf(res, "%d", found);
        write(pipe_pere_fils[1], res, sizeof(res));
        close(pipe_pere_fils[1]);

        // Lire le résultat du fils pour affichage
        int r;
        read(pipe_fils_pere[0], &r, sizeof(int));

        printf("Père : Le mot '%s' %s dans le fichier.\n", mot, found ? "est" : "n'est pas");
    }

    return (0);
}