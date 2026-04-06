#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) 
    {
        printf("Usage : %s nombre1 nombre2\n", argv[0]);
        return (1);
    }

    int a = atoi(argv[1]);  // Convertir le premier argument en entier
    int b = atoi(argv[2]);  // Convertir le deuxième argument en entier

    int somme = a + b;

    printf("La somme de %d et %d est %d\n", a, b, somme);

    return (0);
}