import threading
import random
import time
import sys

compteur = 0
limite = int(sys.argv[1]) if len(sys.argv) > 1 else 10
lock = threading.Lock()

def incrementer():
    global compteur
    while True:
        time.sleep(random.randint(1, 5))
        with lock:
            if compteur >= limite:
                break
            compteur += 1

def afficher():
    global compteur
    while True:
        time.sleep(2)
        with lock:
            if compteur >= limite:
                break
            print("Compteur =", compteur)

th_inc = threading.Thread(target=incrementer)
th_aff = threading.Thread(target=afficher)

th_inc.start()
th_aff.start()

th_inc.join()
th_aff.join()

print("Compteur final =", compteur)