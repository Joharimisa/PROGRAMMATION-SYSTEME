#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9090
#define MAX_CLIENTS 100
#define MAX_LINE 256

typedef struct
{
    char ip[50];
    char nom[50];
} ClientAutorise;

// ==========================
// Charger IP autorisées
// ==========================
int charger_ip_autorisees(const char *filename, ClientAutorise *tab, int max) 
{
    FILE *f = fopen(filename, "r");
    if (!f) 
    {
    
        perror("Erreur ouverture fichier");
        return -1;
    }

    int i = 0;
    while (i < max && fscanf(f, "%s %s", tab[i].ip, tab[i].nom) == 2) 
    {
        i++;
    }

    fclose(f);
    return i;
}

// ==========================
// Chercher IP
// ==========================
char* chercher_ip(ClientAutorise *tab, int n, const char *ip) 
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (strcmp(tab[i].ip, ip) == 0) 
        {
            return tab[i].nom; // retourne le nom
        }
    }
    return (NULL);
}

// ==========================
// Serveur principal
// ==========================
int main() 
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    ClientAutorise clients[MAX_CLIENTS];
    int n_clients = charger_ip_autorisees("add_autoris.txt", clients, MAX_CLIENTS);

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

    printf("Serveur IP en attente sur le port %d...\n", PORT);

    while (1) 
    {
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
        if (client_fd < 0) 
        {
            perror("accept");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));
        printf("Connexion de %s\n", client_ip);

        char *nom_client = chercher_ip(clients, n_clients, client_ip);

        if (nom_client) 
        {
            char msg[100];
            snprintf(msg, sizeof(msg), "Bienvenue %s !\n", nom_client);
            write(client_fd, msg, strlen(msg));
        }
        else 
        {
            char *msg = "Adresse IP non autorisée. Connexion refusée.\n";
            write(client_fd, msg, strlen(msg));
            printf("IP non autorisée : %s\n", client_ip);
            close(client_fd);
            continue;
        }

        // Ici, on peut ajouter d'autres interactions avec le client
        char *msg2 = "Vous êtes connecté au serveur.\n";
        write(client_fd, msg2, strlen(msg2));

        close(client_fd);
    }

    close(server_fd);
    return (0);
}