#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 13000
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct
{
    int sock;
    char ip[INET_ADDRSTRLEN];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;

int current_speaker = -1; // index du client qui peut parler (-1 = personne)

// ==========================
// Envoyer un message à tous les clients sauf l'émetteur
// ==========================
void broadcast(int sender_index, const char *msg)
{
    int i;
    pthread_mutex_lock(&clients_mutex);
    for (i = 0; i < client_count; i++)
    {
        if (i != sender_index)
        {
            pthread_mutex_lock(&write_mutex);
            write(clients[i].sock, msg, strlen(msg));
            pthread_mutex_unlock(&write_mutex);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// ==========================
// Thread pour chaque client
// ==========================
void *client_handler(void *arg)
{
    int idx = *(int*)arg;
    char buffer[BUFFER_SIZE];
    int n;

    while (1)
    {
        // Attendre que ce client puisse parler
        while (current_speaker != idx)
        {
            usleep(100000); // 0.1s
        }

        // Demande au client d'envoyer un message
        pthread_mutex_lock(&write_mutex);
        write(clients[idx].sock, "Vous pouvez parler : ", 21);
        pthread_mutex_unlock(&write_mutex);

        n = read(clients[idx].sock, buffer, sizeof(buffer)-1);
        if (n <= 0)
        {
            printf("Client %s déconnecté\n", clients[idx].ip);
            break;
        }
        buffer[n] = '\0';

        // Affichage sur serveur
        printf("[%s] %s\n", clients[idx].ip, buffer);

        // Diffuser aux autres clients
        char msg_to_send[BUFFER_SIZE + 50];
        snprintf(msg_to_send, sizeof(msg_to_send), "[%s] %s", clients[idx].ip, buffer);
        broadcast(idx, msg_to_send);

        // Passer la parole au prochain client
        pthread_mutex_lock(&clients_mutex);
        current_speaker = (current_speaker + 1) % client_count;
        pthread_mutex_unlock(&clients_mutex);
    }

    close(clients[idx].sock);
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
    int i;

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

    printf("Serveur de forum de chat en attente sur le port %d...\n", PORT);

    while (1)
    {
        new_sock = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
        if (new_sock < 0)
        {
            perror("accept");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);
        clients[client_count].sock = new_sock;
        inet_ntop(AF_INET, &addr.sin_addr, clients[client_count].ip, sizeof(clients[client_count].ip));
        printf("Client connecté : %s\n", clients[client_count].ip);

        // Si c'est le premier client, il commence à parler
        if (client_count == 0)
        {
            current_speaker = 0;
        }

        int idx = client_count;
        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, (void*)&idx);
        pthread_detach(tid);

        client_count++;
        pthread_mutex_unlock(&clients_mutex);
    }

    close(server_fd);
    return 0;
}