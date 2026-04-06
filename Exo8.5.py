import socket
import threading

HOST = "0.0.0.0"
PORT = 12000
BUFFER_SIZE = 1024

clients = []
clients_lock = threading.Lock()

# ==========================
# Fonction pour gérer la paire
# ==========================
def handle_pair(c1, c2):
    while True:
        try:
            msg = c1.recv(BUFFER_SIZE)
            if not msg:
                break
            c2.sendall(b"L'autre client dit : " + msg + b"\nSaisissez la réponse : ")
        except:
            break

    c1.close()
    c2.close()

# ==========================
# Fonction pour chaque client
# ==========================
def client_handler(conn):
    global clients
    with clients_lock:
        clients.append(conn)
        if len(clients) % 2 == 0:
            c1 = clients[-2]
            c2 = clients[-1]
            threading.Thread(target=handle_pair, args=(c1,c2), daemon=True).start()

# ==========================
# Serveur principal
# ==========================
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(5)
    print(f"Serveur de chat en attente sur le port {PORT}...")

    while True:
        conn, addr = s.accept()
        threading.Thread(target=client_handler, args=(conn,), daemon=True).start()