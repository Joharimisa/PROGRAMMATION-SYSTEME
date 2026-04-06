#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE_THRESHOLD 1048576 // 1 Mo en octets

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <dir1> [<dir2> ...]\n", argv[0]);
        return (1);
    }

    for (int i = 1; i < argc; i++)
    {
        DIR *dir = opendir(argv[i]);
        if (!dir) 
        {
            perror(argv[i]);
            continue;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) 
        {
            if (entry->d_type == DT_REG) 
            { 
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", argv[i], entry->d_name);

                struct stat st;
                if (stat(path, &st) == 0)
                {
                    if (st.st_size > SIZE_THRESHOLD)
                    {
                        printf("Fichier : %s, UID : %d, Taille : %ld\n",
                               path, st.st_uid, st.st_size);
                    }
                }
            }
        }
        closedir(dir);
    }
    return (0);
}