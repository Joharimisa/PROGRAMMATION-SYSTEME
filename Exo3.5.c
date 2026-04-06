#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void open_files_in_emacs() 
{
    char filename[256];

    while (1) 
    {
        printf("Entrez le nom d'un fichier texte (ou 'quit' pour quitter) : ");
        if (fgets(filename, sizeof(filename), stdin) == NULL) 
        {
            break;
        }    

        // Supprimer le \n
        filename[strcspn(filename, "\n")] = 0;

        if (strcmp(filename, "quit") == 0) 
        {
            printf("Fin du programme.\n");
            break;
        }

        if (strlen(filename) == 0)
        {
        
            printf("Nom de fichier vide !\n");
            continue;
        }

        char command[300];
        snprintf(command, sizeof(command), "/usr/bin/emacs \"%s\"", filename);
        system(command);
    }
}

int main() 
{
    open_files_in_emacs();
    return (0);
}