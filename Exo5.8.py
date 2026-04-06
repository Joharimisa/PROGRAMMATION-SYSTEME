import threading
import time
import random

sem_T1 = threading.Semaphore(0)
sem_T2 = threading.Semaphore(0)

def T1():
    for i in range(10):
        time.sleep(2)
        print(f"T1 terminé itération {i}")

        sem_T2.release()  # signaler T2
        sem_T1.acquire()  # attendre T2

        print("T1 reprend")

def T2():
    for i in range(10):
        time.sleep(random.randint(4, 9))
        print(f"T2 terminé itération {i}")

        sem_T1.release()  # signaler T1
        sem_T2.acquire()  # attendre T1

        print("T2 reprend")

t1 = threading.Thread(target=T1)
t2 = threading.Thread(target=T2)

t1.start()
t2.start()

t1.join()
t2.join()