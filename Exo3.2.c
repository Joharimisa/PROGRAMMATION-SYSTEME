#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Usage: %s chemin_du_répertoire\n", argv[0]);
        return (1);
    }

    char *dest = argv[1];
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    if (!d) 
    {
        perror("Erreur ouverture répertoire courant");
        return (1);
    }

    // Créer un sous-répertoire dans la destination
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *dirname = strrchr(cwd, '/');
    if (dirname) dirname++;
    else dirname = cwd;

    char dest_path[1024];
    snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, dirname);

    mkdir(dest_path, 0755); // créer le sous-répertoire

    while ((dir = readdir(d)) != NULL) 
    {
        if (dir->d_type == DT_REG)
        { // fichier régulier
            char src_file[1024], dest_file[1024];
            snprintf(src_file, sizeof(src_file), "%s", dir->d_name);
            snprintf(dest_file, sizeof(dest_file), "%s/%s", dest_path, dir->d_name);
            
            char cmd[2048];
            snprintf(cmd, sizeof(cmd), "cp %s %s", src_file, dest_file);
            system(cmd);  // copie du fichier
        }
    }

    closedir(d);
    printf("Répertoire courant copié dans %s\n", dest_path);
    return (0);
}