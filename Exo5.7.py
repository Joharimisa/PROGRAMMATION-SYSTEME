import threading
import time
import random

u = 1.0
compteur = 0
lock = threading.Lock()

def f1(x):
    return 0.25 * (x - 1)**2

def f2(x):
    return (1/6) * (x - 2)**2

def thread1():
    global u, compteur
    while True:
        time.sleep(random.randint(1, 5))
        with lock:
            compteur += 1
            u = f1(u)
            print(f"T1 -> compteur={compteur}, u={u:.6f}")

def thread2():
    global u, compteur
    while True:
        time.sleep(random.randint(1, 5))
        with lock:
            compteur += 1
            u = f2(u)
            print(f"T2 -> compteur={compteur}, u={u:.6f}")

t1 = threading.Thread(target=thread1)
t2 = threading.Thread(target=thread2)

t1.start()
t2.start()

t1.join()
t2.join()