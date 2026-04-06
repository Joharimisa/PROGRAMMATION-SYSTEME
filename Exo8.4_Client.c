#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 11000
#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    FILE *f;
    int n;
    char archive_temp[256];

    if (argc != 3)
    {
        printf("Usage: %s <serveur_ip> <chemin_repertoire>\n", argv[0]);
        return 1;
    }

    char *serveur_ip = argv[1];
    char *repertoire = argv[2];

    struct stat st;
    if (stat(repertoire, &st) != 0 || !S_ISDIR(st.st_mode))
    {
        printf("Le chemin spécifié n'est pas un répertoire\n");
        return 1;
    }

    // Créer l'archive temporaire
    snprintf(archive_temp, sizeof(archive_temp), "/tmp/rep.tgz");
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "tar zcvf %s -C %s .", archive_temp, repertoire);
    system(cmd);

    // Création socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, serveur_ip, &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return 1;
    }

    // Envoyer le nom de l'archive (optionnel)
    write(sock, archive_temp, strlen(archive_temp));

    // Envoyer le contenu de l'archive
    f = fopen(archive_temp, "rb");
    if (!f)
    {
        perror("fopen");
        close(sock);
        return 1;
    }

    while ((n = fread(buffer, 1, sizeof(buffer), f)) > 0)
    {
        write(sock, buffer, n);
    }

    fclose(f);
    close(sock);

    // Supprimer l'archive temporaire
    snprintf(cmd, sizeof(cmd), "rm %s", archive_temp);
    system(cmd);

    printf("Répertoire envoyé avec succès.\n");

    return 0;
}