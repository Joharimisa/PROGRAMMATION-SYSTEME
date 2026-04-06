import os

r, w = os.pipe()

pid = os.fork()
if pid == 0:
    # -------- Fils --------
    os.close(w)  # fermer écriture côté fils

    # Passer fd via variable d'environnement
    env = os.environ.copy()
    env["FD_TUBE"] = str(r)

    os.execvpe("./fils_env.py", ["./fils_env.py"], env)
else:
    # -------- Père --------
    os.close(r)
    message = "Bonjour du père via env!"
    os.write(w, message.encode() + b"\0")
    os.close(w)