import socket

HOST = "0.0.0.0"
PORT = 9090  # port du serveur
AUTORIS_FILE = "add_autoris.txt"

# =========================
# Charger IP autorisées
# =========================
def charger_ip_autorisees(filename):
    ip_dict = {}  # clé = IP, valeur = nom
    with open(filename, "r") as f:
        for ligne in f:
            parts = ligne.strip().split()
            if len(parts) == 2:
                ip, nom = parts
                ip_dict[ip] = nom
    return ip_dict

# =========================
# Communication utilitaire
# =========================
def envoyer(conn, msg):
    conn.sendall(msg.encode())

def recevoir(conn):
    data = conn.recv(1024).decode().strip()
    return data

# =========================
# Serveur principal
# =========================
def main():
    ip_autorisees = charger_ip_autorisees(AUTORIS_FILE)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen(5)
        print(f"Serveur IP en attente sur le port {PORT}...")

        while True:
            conn, addr = s.accept()
            client_ip = addr[0]
            print(f"Connexion de {client_ip}")

            with conn:
                if client_ip in ip_autorisees:
                    nom_client = ip_autorisees[client_ip]
                    envoyer(conn, f"Bienvenue {nom_client} !\n")
                else:
                    envoyer(conn, "Adresse IP non autorisée. Connexion refusée.\n")
                    print(f"IP non autorisée : {client_ip}")
                    continue  # on ferme la connexion

                # Ici, on peut ajouter d'autres interactions avec le client
                envoyer(conn, "Vous êtes connecté au serveur.\n")

if __name__ == "__main__":
    main()