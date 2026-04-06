#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/stat.h>

#define PORT 10000
#define BUFFER_SIZE 1024

int main() 
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    char buffer[BUFFER_SIZE];

    // Création socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    { 
        perror("socket"); exit(1); 
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    { 
        perror("bind"); exit(1); 
    }
    if (listen(server_fd, 5) < 0) 
    { 
        perror("listen"); exit(1); 
    }

    printf("Serveur en attente sur le port %d...\n", PORT);

    while (1) 
    {
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
        if (client_fd < 0) 
        {
             perror("accept"); continue; 
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));
        printf("Connexion de %s\n", client_ip);

        // Recevoir nom du fichier
        int n = read(client_fd, buffer, sizeof(buffer)-1);
        if (n <= 0) 
        { 
            close(client_fd); continue; 
        }
        buffer[n] = '\0';
        char filename_client[256];
        strcpy(filename_client, buffer);

        // Créer le nom de fichier cible
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char nom_cible[512];
        snprintf(nom_cible, sizeof(nom_cible), "/home/save/%s_%04d_%02d_%02d.txt",
                 client_ip,
                 tm_info->tm_year + 1900,
                 tm_info->tm_mon + 1,
                 tm_info->tm_mday);

        // Créer le répertoire /home/save si nécessaire
        mkdir("/home/save", 0777);

        FILE *f = fopen(nom_cible, "w");
        if (!f) 
        {
             perror("fopen"); close(client_fd); continue; 
        }

        // Recevoir contenu du fichier
        while ((n = read(client_fd, buffer, sizeof(buffer))) > 0) 
        {
            fwrite(buffer, 1, n, f);
        }

        fclose(f);
        printf("Fichier reçu et sauvegardé : %s\n", nom_cible);
        close(client_fd);
    }

    close(server_fd);
    return (0);
}