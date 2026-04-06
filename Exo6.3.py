import struct
import os

n = int(input("Nombre d'entiers : "))
tab = []
for i in range(n):
    x = int(input(f"Entier {i} : "))
    tab.append(x)

# Sauvegarde binaire
with open("tableau.bin", "wb") as f:
    for x in tab:
        f.write(struct.pack("i", x))  # 'i' = int 4 octets

# Changer les permissions : rw pour le groupe, r pour les autres
os.chmod("tableau.bin", 0o640)

print("Tableau sauvegardé dans tableau.bin")