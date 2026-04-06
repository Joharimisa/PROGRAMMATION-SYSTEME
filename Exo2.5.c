#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    pid_t pid1, pid2;
    int wait_time;
    time_t start_time, end_time;

    srand(time(NULL));  // Générateur aléatoire

    start_time = time(NULL);  // Temps de départ

    pid1 = fork();
    if (pid1 < 0) 
    {
        perror("Erreur de création de fork");
        return (1);
    } else if (pid1 == 0) 
    {
        wait_time = rand() % 10 + 1;  // Entre 1 et 10 secondes
        printf("Fils 1 attend %d secondes\n", wait_time);
        sleep(wait_time);
        printf("Fils 1 terminé\n");
        return (0);
    }

    pid2 = fork();
    if (pid2 < 0) 
    {
        perror("Erreur de création de fork");
        return (1);
    } else if (pid2 == 0) 
    {
        wait_time = rand() % 10 + 1;  // Entre 1 et 10 secondes
        printf("Fils 2 attend %d secondes\n", wait_time);
        sleep(wait_time);
        printf("Fils 2 terminé\n");
        return (0);
    }

    // Père attend les deux fils
    wait(NULL);
    wait(NULL);

    end_time = time(NULL);  // Temps de fin
    printf("Durée totale : %ld secondes\n", end_time - start_time);

    return (0);
}