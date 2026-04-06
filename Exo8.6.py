import socket
import threading
import time

HOST = "0.0.0.0"
PORT = 13000
BUFFER_SIZE = 1024

clients = []
clients_lock = threading.Lock()
write_lock = threading.Lock()
current_speaker = 0

# ==========================
# Broadcast message
# ==========================
def broadcast(sender_index, msg):
    with clients_lock:
        for i, c in enumerate(clients):
            if i != sender_index:
                with write_lock:
                    c.sendall(msg)

# ==========================
# Thread pour chaque client
# ==========================
def client_handler(idx):
    global current_speaker
    c = clients[idx]
    while True:
        while current_speaker != idx:
            time.sleep(0.1)

        with write_lock:
            c.sendall(b"Vous pouvez parler : ")

        try:
            msg = c.recv(BUFFER_SIZE)
            if not msg:
                break
        except:
            break

        print(f"[{c.getpeername()[0]}] {msg.decode().strip()}")
        broadcast(idx, f"[{c.getpeername()[0]}] {msg.decode()}".encode())

        with clients_lock:
            current_speaker = (current_speaker + 1) % len(clients)

    c.close()

# ==========================
# Serveur principal
# ==========================
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(5)
    print(f"Serveur de forum de chat en attente sur le port {PORT}...")

    while True:
        conn, addr = s.accept()
        with clients_lock:
            clients.append(conn)
        idx = len(clients) - 1
        threading.Thread(target=client_handler, args=(idx,), daemon=True).start()