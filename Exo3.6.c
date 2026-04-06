#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) 
    {
        printf("Usage: %s n k\n", argv[0]);
        return (1);
    }

    int n = atoi(argv[1]);
    int k = atoi(argv[2]);

    // Cas de base : C0^i = 1 ou C_i^i = 1
    if (k == 0 || k == n)
    {
        printf("C(%d,%d) = 1\n", n, k);
        return (0);
    }

    // Fichiers temporaires pour stocker C(k, n-1) et C(k-1, n-1)
    char file1[256], file2[256];
    snprintf(file1, sizeof(file1), "/tmp/C_%d_%d.tmp", n-1, k);
    snprintf(file2, sizeof(file2), "/tmp/C_%d_%d.tmp", n-1, k-1);

    // Appel au programme lui-même via system pour C(k, n-1)
    char cmd1[512], cmd2[512];
    snprintf(cmd1, sizeof(cmd1), "%s %d %d > %s", argv[0], n-1, k, file1);
    snprintf(cmd2, sizeof(cmd2), "%s %d %d > %s", argv[0], n-1, k-1, file2);

    system(cmd1);
    system(cmd2);

    // Lire les valeurs
    int val1, val2;
    FILE *f = fopen(file1, "r");
    fscanf(f, "C(%*d,%*d) = %d", &val1);
    fclose(f);

    f = fopen(file2, "r");
    fscanf(f, "C(%*d,%*d) = %d", &val2);
    fclose(f);

    // Supprimer fichiers temporaires
    remove(file1);
    remove(file2);

    int result = val1 + val2;
    printf("C(%d,%d) = %d\n", n, k, result);
    return (0);
}