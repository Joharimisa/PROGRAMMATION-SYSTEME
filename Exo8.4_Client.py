import socket
import sys
import os
import tarfile

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <serveur_ip> <chemin_repertoire>")
    sys.exit(1)

serveur_ip = sys.argv[1]
repertoire = sys.argv[2]

if not os.path.isdir(repertoire):
    print("Le chemin spécifié n'est pas un répertoire")
    sys.exit(1)

# Créer archive temporaire
archive_temp = "/tmp/rep.tgz"
with tarfile.open(archive_temp, "w:gz") as tar:
    tar.add(repertoire, arcname=os.path.basename(repertoire))

# Envoyer archive au serveur
PORT = 11000
BUFFER_SIZE = 4096

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((serveur_ip, PORT))

    # Envoyer le nom de l'archive (optionnel)
    s.sendall(archive_temp.encode())

    # Envoyer le contenu de l'archive
    with open(archive_temp, "rb") as f:
        while True:
            data = f.read(BUFFER_SIZE)
            if not data:
                break
            s.sendall(data)

print("Répertoire envoyé sous forme d'archive.")
os.remove(archive_temp)