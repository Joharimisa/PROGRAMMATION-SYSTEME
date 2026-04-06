import socket
import time

HOST = "0.0.0.0"
PORT = 8080

# =========================
# a) Structure Produit
# =========================
class Produit:
    def __init__(self, nom, prix):
        self.nom = nom
        self.prix = float(prix)

# =========================
# b) Charger base produits
# =========================
def charger_produits(filename):
    produits = []
    with open(filename, "r") as f:
        for ligne in f:
            nom, prix = ligne.strip().split()
            produits.append(Produit(nom, prix))
    return produits

# =========================
# c) Recherche produit
# =========================
def chercher_produit(produits, nom):
    for p in produits:
        if p.nom == nom:
            return p
    return None

# =========================
# f) Sauvegarde commande
# =========================
def sauvegarder_commande(client_nom, details, total):
    t = time.localtime()
    filename = time.strftime("commande_%Y%m%d_%H%M%S.txt", t)

    with open(filename, "w") as f:
        f.write("=== COMMANDE ===\n")
        f.write(f"Client: {client_nom}\n\n")
        f.write(details)
        f.write(f"\nTOTAL = {total:.2f}\n")

# =========================
# Communication utilitaire
# =========================
def envoyer(conn, msg):
    conn.sendall(msg.encode())

def recevoir(conn):
    data = conn.recv(1024).decode().strip()
    return data

# =========================
# d,e) Serveur principal
# =========================
def main():
    produits = charger_produits("produits.txt")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen(1)

        print(f"Serveur en attente sur le port {PORT}...")

        conn, addr = s.accept()
        with conn:
            print("Client connecté :", addr)

            envoyer(conn, "Bienvenue ! Entrez votre nom :\n")
            client_nom = recevoir(conn)

            total = 0
            details = ""

            while True:
                envoyer(conn, "\nProduit (tapez stop pour finir) :\n")
                nom = recevoir(conn)

                if nom.lower() == "stop":
                    break

                envoyer(conn, "Quantité :\n")
                try:
                    qte = int(recevoir(conn))
                except:
                    envoyer(conn, "Quantité invalide\n")
                    continue

                produit = chercher_produit(produits, nom)

                if produit:
                    prix = produit.prix * qte
                    total += prix

                    ligne = f"{nom} x{qte} = {prix:.2f}\n"
                    details += ligne

                    envoyer(conn, "Ajouté à la commande\n")
                else:
                    envoyer(conn, "Produit introuvable\n")

            envoyer(conn, f"\nTOTAL = {total:.2f}\n")

            sauvegarder_commande(client_nom, details, total)

            envoyer(conn, "Commande enregistrée.\n")

if __name__ == "__main__":
    main()