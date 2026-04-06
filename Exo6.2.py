import os
import sys

SIZE_THRESHOLD = 1024 * 1024  # 1 Mo

if len(sys.argv) < 2:
    print(f"Usage: {sys.argv[0]} <dir1> [<dir2> ...]")
    sys.exit(1)

for dir_path in sys.argv[1:]:
    if not os.path.isdir(dir_path):
        print(f"{dir_path} n'est pas un répertoire")
        continue

    for entry in os.scandir(dir_path):
        if entry.is_file():
            stat = entry.stat()
            if stat.st_size > SIZE_THRESHOLD:
                print(f"Fichier : {entry.path}, UID : {stat.st_uid}, Taille : {stat.st_size}")