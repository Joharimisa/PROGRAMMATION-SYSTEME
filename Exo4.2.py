import subprocess

def fils():
    mot = input("Fils : Entrez un mot : ")
    return mot

if __name__ == "__main__":
    # Fils simulé via subprocess.communicate
    process = subprocess.Popen(
        ["python3", "-c", "mot = input(); print(mot)"], 
        stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True
    )

    mot, _ = process.communicate()
    mot = mot.strip()

    # Père cherche le mot
    found = False
    with open("fichier.txt") as f:
        for word in f.read().split():
            if word == mot:
                found = True
                break

    print(f"Père : Le mot '{mot}' {'est' if found else 'n\'est pas'} dans le fichier.")