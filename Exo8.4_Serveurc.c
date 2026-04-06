#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/stat.h>

#define PORT 11000
#define BUFFER_SIZE 4096
#define SAVE_DIR "/home/save_rep/"

int main()
{
    int server_fd;
    int client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char buffer[BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];
    time_t t;
    struct tm *tm_info;
    char archive_save[512];
    FILE *f;

    // Création du répertoire de sauvegarde
    mkdir(SAVE_DIR, 0777);

    // Création socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    addrlen = sizeof(addr);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(1);
    }

    printf("Serveur en attente sur le port %d...\n", PORT);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));
        printf("Connexion de %s\n", client_ip);

        // Recevoir le nom de l'archive envoyée (optionnel)
        int n;
        n = read(client_fd, buffer, sizeof(buffer)-1);
        if (n <= 0)
        {
            close(client_fd);
            continue;
        }
        buffer[n] = '\0';

        // Créer le nom de sauvegarde final : IP + date
        t = time(NULL);
        tm_info = localtime(&t);
        snprintf(archive_save, sizeof(archive_save), "%s%s_%04d_%02d_%02d.tgz",
                 SAVE_DIR, client_ip,
                 tm_info->tm_year + 1900,
                 tm_info->tm_mon + 1,
                 tm_info->tm_mday);

        // Ouvrir le fichier pour écrire l'archive reçue
        f = fopen(archive_save, "wb");
        if (!f)
        {
            perror("fopen");
            close(client_fd);
            continue;
        }

        // Recevoir l'archive
        while ((n = read(client_fd, buffer, sizeof(buffer))) > 0)
        {
            fwrite(buffer, 1, n, f);
        }

        fclose(f);

        // Décompresser l'archive et supprimer le fichier .tgz
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "cd %s; tar zxvf %s; rm %s", SAVE_DIR, archive_save, archive_save);
        system(cmd);

        printf("Répertoire reçu et décompressé dans %s\n", SAVE_DIR);

        close(client_fd);
    }

    close(server_fd);
    return 0;
}