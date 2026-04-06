#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void copy_current_dir_to_targets()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char dirname[256];
    strcpy(dirname, strrchr(cwd, '/') ? strrchr(cwd, '/') + 1 : cwd);

    char target[1024];
    DIR *d;
    struct dirent *dir;

    while (1) 
    {
        printf("Entrez un répertoire cible (ou 'quit' pour quitter) : ");
        if (fgets(target, sizeof(target), stdin) == NULL) break;
        target[strcspn(target, "\n")] = 0;  // enlever le \n

        if (strcmp(target, "quit") == 0) 
        {
            printf("Fin du programme.\n");
            break;
        }

        // Créer le répertoire cible s'il n'existe pas
        mkdir(target, 0755);

        // Créer sous-répertoire pour le nom du répertoire courant
        char dest_path[1024];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", target, dirname);
        mkdir(dest_path, 0755);

        // Parcourir fichiers du répertoire courant et copier
        d = opendir(cwd);
        if (!d) 
        { 
            perror("Erreur ouverture répertoire"); 
            continue;
        }

        while ((dir = readdir(d)) != NULL) 
        {
            if (dir->d_type == DT_REG) 
            {  // fichiers réguliers
                char src_file[1024], dest_file[1024];
                snprintf(src_file, sizeof(src_file), "%s/%s", cwd, dir->d_name);
                snprintf(dest_file, sizeof(dest_file), "%s/%s", dest_path, dir->d_name);
                char cmd[2048];
                snprintf(cmd, sizeof(cmd), "cp \"%s\" \"%s\"", src_file, dest_file);
                system(cmd);
            }
        }

        closedir(d);
        printf("Répertoire courant copié dans %s\n", dest_path);
    }
}

int main()
{
    copy_current_dir_to_targets();
    return (0);
}