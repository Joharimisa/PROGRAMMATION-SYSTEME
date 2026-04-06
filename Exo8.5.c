#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12000
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct
{
    int sock;
    int paired_sock; // socket du client avec qui il est en pair
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// ==========================
// Envoyer un message
// ==========================
void send_message(int sock, const char *msg)
{
    write(sock, msg, strlen(msg));
}

// ==========================
// Thread de communication entre deux clients
// ==========================
void *handle_pair(void *arg)
{
    Client *c = (Client*)arg;
    char buffer[BUFFER_SIZE];
    int n;

    while (1)
    {
        // Recevoir message de client c->sock
        n = read(c->sock, buffer, sizeof(buffer)-1);
        if (n <= 0)
        {
            printf("Client déconnecté\n");
            break;
        }
        buffer[n] = '\0';

        // Envoyer message au client pair
        char msg_to_pair[BUFFER_SIZE + 50];
        snprintf(msg_to_pair, sizeof(msg_to_pair), "L'autre client dit : %s\nSaisissez la réponse : ", buffer);
        send_message(c->paired_sock, msg_to_pair);
    }

    close(c->sock);
    close(c->paired_sock);
    pthread_exit(NULL);
}

// ==========================
// Thread pour chaque client
// ==========================
void *client_handler(void *arg)
{
    int sock = *(int*)arg;
    pthread_mutex_lock(&clients_mutex);

    clients[client_count].sock = sock;
    clients[client_count].paired_sock = -1;
    client_count++;

    // Vérifier si on peut créer une paire
    if (client_count % 2 == 0)
    {
        clients[client_count-1].paired_sock = clients[client_count-2].sock;
        clients[client_count-2].paired_sock = clients[client_count-1].sock;

        // Lancer le thread de conversation pour cette paire
        pthread_t tid;
        pthread_create(&tid, NULL, handle_pair, (void*)&clients[client_count-2]);
    }

    pthread_mutex_unlock(&clients_mutex);
    pthread_exit(NULL);
}

// ==========================
// Serveur principal
// ==========================
int main()
{
    int server_fd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    int new_sock;

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

    printf("Serveur de chat en attente sur le port %d...\n", PORT);

    while (1)
    {
        new_sock = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
        if (new_sock < 0)
        {
            perror("accept");
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, (void*)&new_sock);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}