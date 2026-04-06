import threading
import random
import time

SIZE = 10
buffer = []
lock = threading.Lock()

vide = threading.Semaphore(SIZE)
plein = threading.Semaphore(0)

def producteur(id):
    while True:
        time.sleep(random.randint(1,3))
        item = chr(random.randint(65, 90))

        vide.acquire()
        with lock:
            buffer.append(item)  # LIFO
            print(f"Producteur {id} produit {item} (taille={len(buffer)})")
        plein.release()

def consommateur(id):
    while True:
        time.sleep(random.randint(1,3))

        plein.acquire()
        with lock:
            item = buffer.pop()  # LIFO
            print(f"Consommateur {id} consomme {item} (taille={len(buffer)})")
        vide.release()

N = int(input("Nombre de producteurs : "))
M = int(input("Nombre de consommateurs : "))

threads = []

for i in range(N):
    t = threading.Thread(target=producteur, args=(i,))
    threads.append(t)
    t.start()

for i in range(M):
    t = threading.Thread(target=consommateur, args=(i,))
    threads.append(t)
    t.start()

for t in threads:
    t.join()