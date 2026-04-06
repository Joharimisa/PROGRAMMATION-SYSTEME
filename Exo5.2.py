import threading
import random

SIZE = 10

def generer_tableau(result):
    tab = [random.randint(0, 99) for _ in range(SIZE)]
    result.extend(tab)  # retourner via liste partagée

# Liste vide pour récupérer le résultat
result = []

t = threading.Thread(target=generer_tableau, args=(result,))
t.start()
t.join()

print("Tableau généré par le thread :", result)