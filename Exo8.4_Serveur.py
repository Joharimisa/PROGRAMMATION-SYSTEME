import socket
import os
import tarfile
import time

HOST = "0.0.0.0"
PORT = 11000
SAVE_DIR = "/home/save_repertoires/"

os.makedirs(SAVE_DIR, exist_ok=True)
BUFFER_SIZE = 4096

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(5)
    print(f"Serveur en attente sur le port {PORT}...")

    while True:
        conn, addr = s.accept()
        client_ip = addr[0]
        print(f"Connexion de {client_ip}")

        with conn:
            # Recevoir le nom de l archive envoyée
            archive_nom = conn.recv(1024).decode().strip()

            # Nom de sauvegarde final avec IP + date
            t = time.localtime()
            archive_save = os.path.join(SAVE_DIR,
                f"{client_ip}_{t.tm_year}_{t.tm_mon:02d}_{t.tm_mday:02d}.tgz")

            # Recevoir l archive
            with open(archive_save, "wb") as f:
                while True:
                    data = conn.recv(BUFFER_SIZE)
                    if not data:
                        break
                    f.write(data)

            # Décompresser l archive dans le répertoire
            with tarfile.open(archive_save, "r:gz") as tar:
                tar.extractall(path=SAVE_DIR)
            os.remove(archive_save)

            print(f"Répertoire reçu et décompressé dans {SAVE_DIR}")