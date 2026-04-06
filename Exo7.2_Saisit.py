import os
import signal

pid = int(input("PID du programme affiche : "))
x = int(input("Entrez un entier : "))

with open("/tmp/entier.txt", "w") as f:
    f.write(str(x))

os.kill(pid, signal.SIGUSR1)

print("Signal envoyé")