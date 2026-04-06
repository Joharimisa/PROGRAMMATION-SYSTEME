import threading
import time
import random

buffer = None

vide = threading.Semaphore(1)
plein = threading.Semaphore(0)

def emetteur():
    global buffer
    while True:
        time.sleep(random.randint(1,3))

        vide.acquire()

        buffer = chr(random.randint(65, 90))
        print(f"Émetteur envoie : {buffer}")

        plein.release()

def recepteur():
    global buffer
    while True:
        time.sleep(random.randint(1,3))

        plein.acquire()

        print(f"Récepteur lit : {buffer}")

        vide.release()

t1 = threading.Thread(target=emetteur)
t2 = threading.Thread(target=recepteur)

t1.start()
t2.start()

t1.join()
t2.join()