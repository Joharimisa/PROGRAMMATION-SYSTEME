import os

def open_files_in_emacs():
    while True:
        filename = input("Entrez le nom d'un fichier texte (ou 'quit' pour quitter) : ")

        if filename.lower() == "quit":
            print("Fin du programme.")
            break

        if not filename.strip():
            print("Nom de fichier vide !")
            continue

        os.system(f"/usr/bin/emacs '{filename}'")

# Appel de la fonction
open_files_in_emacs()