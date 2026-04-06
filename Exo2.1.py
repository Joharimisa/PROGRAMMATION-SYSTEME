import os

pid = os.fork()  # Crée un processus fils

if pid == 0:
    # Processus fils
    print("Je suis le fils")
else:
    # Processus père
    print("Je suis le père")