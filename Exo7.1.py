import os
import time
import signal

pid = os.fork()

if pid == 0:
    # FILS
    while True:
        print("Fils en cours de calcul...")
        time.sleep(1)
else:
    # PÈRE
    while True:
        c = input("\nMenu : s (stop), r (resume), q (quit) : ")

        if c == 's':
            os.kill(pid, signal.SIGSTOP)
            print("Fils suspendu")

        elif c == 'r':
            os.kill(pid, signal.SIGCONT)
            print("Fils repris")

        elif c == 'q':
            os.kill(pid, signal.SIGKILL)
            print("Fils tué, fin du programme")
            break