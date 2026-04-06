#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 10000
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
    
        printf("Usage: %s <serveur_ip> <fichier_local>\n", argv[0]);
        return (1);
    }

    char *serveur_ip = argv[1];
    char *fichier_local = argv[2];

    FILE *f = fopen(fichier_local, "r");
    if (!f)
    { 
        perror("fopen"); return 1; 
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    { 
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, serveur_ip, &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
    {
        perror("connect"); 
        return (1);
    }

    // Envoyer le nom du fichier
    write(sock, fichier_local, strlen(fichier_local));

    // Envoyer contenu du fichier
    char buffer[BUFFER_SIZE];
    int n;
    while ((n = fread(buffer, 1, sizeof(buffer), f)) > 0) 
    {
        write(sock, buffer, n);
    }

    fclose(f);
    close(sock);
    printf("Fichier envoyé.\n");
    return (0);
}