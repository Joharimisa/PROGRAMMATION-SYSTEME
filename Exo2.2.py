import os

pid1 = os.fork()  # Création du fils 1

if pid1 == 0:
    print("Je suis le fils 1")
    os._exit(0)  # Terminer le fils 1

pid2 = os.fork()  # Création du fils 2

if pid2 == 0:
    print("Je suis le fils 2")
    os._exit(0)  # Terminer le fils 2

# Processus père
print("Je suis le père")

# Attendre que les deux fils terminent
os.waitpid(pid1, 0)
os.waitpid(pid2, 0)