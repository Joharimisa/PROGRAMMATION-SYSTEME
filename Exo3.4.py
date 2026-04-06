import os
import shutil

def copy_current_dir_to_targets():
    src = os.getcwd()
    dirname = os.path.basename(src)

    while True:
        target = input("Entrez un répertoire cible (ou 'quit' pour quitter) : ")
        if target.lower() == "quit":
            print("Fin du programme.")
            break

        if not os.path.exists(target):
            print(f"Le répertoire '{target}' n'existe pas. Création...")
            os.makedirs(target)

        destination_path = os.path.join(target, dirname)
        shutil.copytree(src, destination_path)
        print(f"Le répertoire courant a été copié dans {destination_path}")

# Appel de la fonction
copy_current_dir_to_targets()