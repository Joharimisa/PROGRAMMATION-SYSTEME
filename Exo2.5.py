import os
import time
import random

start_time = time.time()  # Temps de départ

for i in [1, 2]:
    pid = os.fork()
    if pid == 0:
        wait_time = random.randint(1, 10)
        print(f"Fils {i} attend {wait_time} secondes")
        time.sleep(wait_time)
        print(f"Fils {i} terminé")
        os._exit(0)  # Terminer le fils

# Père attend tous les fils
os.wait()
os.wait()

end_time = time.time()  # Temps de fin
duration = int(end_time - start_time)
print(f"Durée totale : {duration} secondes")