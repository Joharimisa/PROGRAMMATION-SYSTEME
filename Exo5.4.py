import threading
import random
import sys

cancel_flag = False

class Tableau:
    def __init__(self, n):
        self.tab = [0]*n
        self.n = n

def generer_tableau(T):
    for i in range(T.n):
        T.tab[i] = random.randint(0, 99)
        if cancel_flag:
            return

def verifier_inclusion(T1, T2):
    included = all(x in T2.tab for x in T1.tab)
    print(f"T1 {'est inclus dans' if included else 'n\'est pas inclus dans'} T2")

def surveiller_entrée():
    global cancel_flag
    c = input("Appuyez sur 'A' pour annuler : ")
    if c.upper() == 'A':
        cancel_flag = True
        print("Programme annulé par l'utilisateur !")
        sys.exit(0)

# Initialisation
T1 = Tableau(5)
T2 = Tableau(10)

th_cancel = threading.Thread(target=surveiller_entrée)
th_cancel.start()

th1 = threading.Thread(target=generer_tableau, args=(T1,))
th2 = threading.Thread(target=generer_tableau, args=(T2,))

th1.start()
th2.start()

th1.join()
th2.join()

# Vérification inclusion
th_incl = threading.Thread(target=verifier_inclusion, args=(T1, T2))
th_incl.start()
th_incl.join()