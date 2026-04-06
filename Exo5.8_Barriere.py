import threading
import time
import random
import sys

N = int(sys.argv[1]) if len(sys.argv) > 1 else 3

compteur = 0
mutex = threading.Lock()
barriere = threading.Semaphore(0)

def worker(id):
    global compteur

    for i in range(10):
        time.sleep(random.randint(1, 5))
        print(f"Thread {id} terminé itération {i}")

        with mutex:
            compteur += 1
            if compteur == N:
                for _ in range(N):
                    barriere.release()
                compteur = 0

        barriere.acquire()
        print(f"Thread {id} reprend")

threads = []
for i in range(N):
    t = threading.Thread(target=worker, args=(i,))
    threads.append(t)
    t.start()

for t in threads:
    t.join()