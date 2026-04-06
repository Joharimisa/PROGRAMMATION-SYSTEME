n = int(input("Entrez le nombre d'elements n : "))
tab = []

for i in range(n):
    x = int(input(f"tab[{i}] = "))
    tab.append(x)

while True:
    try:
        i = int(input("Entrez l'indice i : "))
        print(f"tab[{i}] = {tab[i]}")
        break
    except IndexError:
        print("Indice invalide ! Ressaisir.")