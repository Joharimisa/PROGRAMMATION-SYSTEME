import threading
import random
import time

SIZE = 15
database = [0]*SIZE

readcount = 0
mutex = threading.Semaphore(1)
wrt = threading.Semaphore(1)

def lecteur(id):
    global readcount
    while True:
        time.sleep(random.randint(1,3))

        mutex.acquire()
        readcount += 1
        if readcount == 1:
            wrt.acquire()
        mutex.release()

        i = random.randint(0, SIZE-1)
        print(f"Lecteur {id} lit database[{i}] = {database[i]}")

        mutex.acquire()
        readcount -= 1
        if readcount == 0:
            wrt.release()
        mutex.release()

def redacteur(id):
    while True:
        time.sleep(random.randint(1,10))

        wrt.acquire()

        i = random.randint(0, SIZE-1)
        database[i] = random.randint(0,255)
        print(f"Rédacteur {id} écrit database[{i}] = {database[i]}")

        wrt.release()

N = int(input("Nombre de lecteurs : "))
M = int(input("Nombre de rédacteurs : "))

threads = []

for i in range(N):
    t = threading.Thread(target=lecteur, args=(i,))
    t.start()
    threads.append(t)

for i in range(M):
    t = threading.Thread(target=redacteur, args=(i,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()