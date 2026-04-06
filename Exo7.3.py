import os
import signal
import time

N = 5
fils = []

def handler_fils(signum, frame):
    print(f"Fils {os.getpid()} : je suis tué")
    exit(0)

def handler_pere(signum, frame):
    print("Père : je suis tué")
    exit(0)

signal.signal(signal.SIGTERM, handler_pere)

# création fils
for _ in range(N):
    pid = os.fork()
    if pid == 0:
        signal.signal(signal.SIGTERM, handler_fils)
        while True:
            print(f"Fils {os.getpid()} actif")
            time.sleep(2)
    else:
        fils.append(pid)

# père
while True:
    choix = input("s: stop, r: resume, k: kill : ")
    index = int(input("Index (0-4): "))

    if choix == 's':
        os.kill(fils[index], signal.SIGSTOP)
    elif choix == 'r':
        os.kill(fils[index], signal.SIGCONT)
    elif choix == 'k':
        os.kill(fils[index], signal.SIGTERM)