#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() 
{
    int n,i;
    printf("Nombre d'entiers à saisir : ");
    scanf("%d", &n);

    int *tab = malloc(n * sizeof(int));
    if (!tab)
    {
         perror("malloc");
         return (1); 
    }

    for (i = 0; i < n; i++) 
    {
        printf("Entier %d : ", i);
        scanf("%d", &tab[i]);
    }

    // Créer le fichier avec permissions rw pour le groupe et r pour les autres
    int fd = open("tableau.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWGRP | S_IROTH);
    if (fd == -1) 
    { 
        perror("open");
        return (1);
    }

    if (write(fd, tab, n * sizeof(int)) != n * sizeof(int)) 
    {
        perror("write");
    }

    close(fd);
    free(tab);
    printf("Tableau sauvegardé dans tableau.bin\n");
    return (0);
}