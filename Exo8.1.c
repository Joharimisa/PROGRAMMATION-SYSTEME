#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 8080
#define MAX_PRODUITS 100
#define MAX_NAME 50
#define BUFFER_SIZE 1024

// =======================
// STRUCTURE PRODUIT
// =======================
typedef struct
{
    char nom[MAX_NAME];
    float prix;
} Produit;

// =======================
// CHARGER PRODUITS
// =======================
int charger_produits(const char *filename, Produit *tab, int max)
{
    FILE *f = fopen(filename, "r");
    int i = 0;
    if (!f) 
    {
        perror("Erreur ouverture fichier");
        exit(1);
    }

    while (i < max && fscanf(f, "%s %f", tab[i].nom, &tab[i].prix) == 2) 
    {
        i++;
    }

    fclose(f);
    return (i);
}

// =======================
// RECHERCHE PRODUIT
// =======================
Produit* chercher_produit(Produit *tab, int n, const char *nom)
{
    int i;
    for (i = 0; i < n; i++) 
    {
        if (strcmp(tab[i].nom, nom) == 0) 
        {
            return &tab[i];
        }
    }
    return (NULL);
}

// =======================
// SAUVEGARDE COMMANDE
// =======================
void sauvegarder_commande(char *details, float total) 
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    char filename[100];
    sprintf(filename, "commande_%04d%02d%02d_%02d%02d%02d.txt",
        tm->tm_year + 1900,
        tm->tm_mon + 1,
        tm->tm_mday,
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec
    );

    FILE *f = fopen(filename, "w");
    if (!f) 
    {
        return;
    }    

    fprintf(f, "=== COMMANDE ===\n");
    fprintf(f, "%s", details);
    fprintf(f, "\nTOTAL = %.2f\n", total);

    fclose(f);
}

// =======================
// MAIN SERVEUR
// =======================
int main() 
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    // Charger base produits
    Produit produits[MAX_PRODUITS];
    int n = charger_produits("produits.txt", produits, MAX_PRODUITS);

    // Création socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Serveur en attente sur port %d...\n", PORT);

    client_fd = accept(server_fd, NULL, NULL);

    write(client_fd, "Bienvenue ! Entrez votre nom :\n", 32);

    read(client_fd, buffer, BUFFER_SIZE);
    buffer[strcspn(buffer, "\n")] = 0;

    char client_nom[50];
    strcpy(client_nom, buffer);

    float total = 0;
    char details[2048] = "";

    while (1) 
    {
        write(client_fd, "\nProduit (tapez stop pour finir) :\n", 38);
        int r = read(client_fd, buffer, BUFFER_SIZE);
        if (r <= 0) 
        {
         break;
        } 

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "stop") == 0)
        {
            break;
        }
        char nom_produit[50];
        strcpy(nom_produit, buffer);

        write(client_fd, "Quantité :\n", 11);
        read(client_fd, buffer, BUFFER_SIZE);

        int qte = atoi(buffer);

        Produit *p = chercher_produit(produits, n, nom_produit);

        if (p) 
        {
            float prix = p->prix * qte;
            total += prix;

            char ligne[200];
            sprintf(ligne, "%s x%d = %.2f\n", nom_produit, qte, prix);
            strcat(details, ligne);

            write(client_fd, "Ajouté à la commande\n", 22);
        }
        else 
        {
            write(client_fd, "Produit introuvable\n", 21);
        }
    }

    // Affichage total
    char msg[100];
    sprintf(msg, "\nTOTAL = %.2f\n", total);
    write(client_fd, msg, strlen(msg));

    // Sauvegarde fichier
    sauvegarder_commande(details, total);

    write(client_fd, "Commande enregistrée.\n", 23);

    close(client_fd);
    close(server_fd);

    return (0);
}