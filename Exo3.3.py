import os

filename = input("Entrez le nom du fichier texte : ")

if not filename:
    print("Nom de fichier vide !")
    exit(1)

# Ouvrir le fichier avec Emacs
os.system(f"/usr/bin/emacs '{filename}'")