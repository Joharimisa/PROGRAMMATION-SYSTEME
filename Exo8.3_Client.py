import socket
import sys
import os

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <serveur_ip> <fichier_local>")
    sys.exit(1)

serveur_ip = sys.argv[1]
fichier_local = sys.argv[2]

if not os.path.exists(fichier_local):
    print("Fichier inexistant")
    sys.exit(1)

PORT = 10000
BUFFER_SIZE = 1024

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((serveur_ip, PORT))
    # Envoyer le nom du fichier
    s.sendall(fichier_local.encode())

    # Envoyer le contenu du fichier
    with open(fichier_local, "rb") as f:
        while True:
            data = f.read(BUFFER_SIZE)
            if not data:
                break
            s.sendall(data)

print("Fichier envoyé.")