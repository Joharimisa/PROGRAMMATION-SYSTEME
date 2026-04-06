import threading
import random
import time

N = 100
tableau = [0.0]*N
lock = threading.Lock()

# initialisation
tableau[0] = 0
tableau[-1] = 0
for i in range(1, N-1):
    tableau[i] = random.uniform(0, 100)

def calcul_moyenne():
    while True:
        with lock:
            for i in range(1, N-1):
                tableau[i] = (tableau[i-1] + tableau[i] + tableau[i+1]) / 3.0
        time.sleep(random.randint(1,3))

def afficher_tableau():
    while True:
        time.sleep(4)
        with lock:
            print(["{:.2f}".format(x) for x in tableau])

th_calc = threading.Thread(target=calcul_moyenne)
th_aff = threading.Thread(target=afficher_tableau)

th_calc.start()
th_aff.start()

th_calc.join()
th_aff.join()