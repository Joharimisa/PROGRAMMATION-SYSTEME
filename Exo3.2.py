import sys
import os
import shutil

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} chemin_du_répertoire")
    sys.exit(1)

dest = sys.argv[1]
src = os.getcwd()  # Répertoire courant

if not os.path.isdir(dest):
    print(f"Le répertoire cible n'existe pas : {dest}")
    sys.exit(1)

dirname = os.path.basename(src)
destination_path = os.path.join(dest, dirname)

shutil.copytree(src, destination_path)

print(f"Le répertoire courant a été copié dans {destination_path}")