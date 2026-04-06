import os

r, w = os.pipe()  # créer le pipe

pid = os.fork()
if pid == 0:
    # -------- Fils --------
    os.close(w)  # fermer écriture
    fd_str = str(r)
    os.execv("./fils.py", ["./fils.py", fd_str])
else:
    # -------- Père --------
    os.close(r)  # fermer lecture
    message = "Bonjour du père !"
    os.write(w, message.encode() + b"\0")
    os.close(w)