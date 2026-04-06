import threading
import random

class TypeTableau:
    def __init__(self, n):
        self.tab = [0]*n
        self.n = n
        self.x = None

def generer_tableau(t):
    t.tab = [random.randint(0, 99) for _ in range(t.n)]
    print("Thread génération : tableau généré.")

def chercher_x(t):
    found = t.x in t.tab
    print(f"Thread recherche : {t.x} {'est' if found else "n'est pas"} dans le tableau.")

# Lecture nombre d'éléments
n = int(input("Nombre d'éléments : "))
t = TypeTableau(n)

# Thread génération
th_gen = threading.Thread(target=generer_tableau, args=(t,))
th_gen.start()

# Pendant ce temps, on lit x
t.x = int(input("Entier x à chercher : "))

th_gen.join()

# Thread recherche
th_search = threading.Thread(target=chercher_x, args=(t,))
th_search.start()
th_search.join()