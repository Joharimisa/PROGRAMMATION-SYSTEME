#include <stdio.h>
#include <stdlib.h>

int main()
{
    char filename[256];

    printf("Entrez le nom du fichier texte : ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) 
    {
        printf("Erreur de lecture\n");
        return (1);
    }

    // Supprimer le \n à la fin de la saisie
    size_t len = 0;
    while (filename[len] != '\0') len++;
    if (len > 0 && filename[len-1] == '\n') filename[len-1] = '\0';

    if (filename[0] == '\0') 
    {
        printf("Nom de fichier vide !\n");
        return (1);
    }

    char command[300];
    snprintf(command, sizeof(command), "/usr/bin/emacs \"%s\"", filename);

    system(command);  // Exécuter la commande

    return (0);
}