import signal
import time

def handler(signum, frame):
    with open("/tmp/entier.txt", "r") as f:
        x = f.read()
    print("Valeur reçue :", x)

print("PID :", os.getpid())

signal.signal(signal.SIGUSR1, handler)

while True:
    time.sleep(1)