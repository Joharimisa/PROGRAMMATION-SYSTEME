import os

for i in range(1, 6):
    pid = os.fork()
    
    if pid == 0:
        # Code du fils
        print(f"Je suis le fils {i}")
        os._exit(0)  # Terminer le fils pour que il ne crée pas d autres fils
        break  # Optionnel, car os._exit termine déjà le fils
    # Le père continue la boucle pour créer les autres fils

# Code du père
for _ in range(5):
    os.wait()  # Attendre tous les fils

print("Je suis le père")