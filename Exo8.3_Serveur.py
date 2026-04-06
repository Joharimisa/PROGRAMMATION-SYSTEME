import socket
import time
import os

HOST = "0.0.0.0"
PORT = 10000
SAVE_DIR = "/home/save/"

os.makedirs(SAVE_DIR, exist_ok=True)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(5)
    print(f"Serveur en attente sur le port {PORT}...")

    while True:
        conn, addr = s.accept()
        client_ip = addr[0]
        print(f"Connexion de {client_ip}")
        with conn:
            # Recevoir le nom du fichier
            filename_client = conn.recv(1024).decode().strip()

            # Nom cible avec IP + date
            t = time.localtime()
            nom_cible = os.path.join(SAVE_DIR,
                f"{client_ip}_{t.tm_year}_{t.tm_mon:02d}_{t.tm_mday:02d}.txt")

            # Recevoir contenu du fichier
            with open(nom_cible, "wb") as f:
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    f.write(data)
            print(f"Fichier reçu et sauvegardé : {nom_cible}")