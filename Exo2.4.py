import os

num_fils = 0 

for i in range(1, 6):
    pid = os.fork()
    
    if pid == 0:
        num_fils = i  # Chaque fils a son numéro
        print(f"Je suis le fils {num_fils}")
        os._exit(0)  # Terminer le fils

# Le père attend tous les fils
for i in range(5):
    os.wait()

print("Je suis le père")