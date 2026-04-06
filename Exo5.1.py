import threading

tab = [1, 2, 3, 4, 5]

def afficher_tableau(tab):
    print("Thread : tableau =", tab)

# Création du thread
t = threading.Thread(target=afficher_tableau, args=(tab,))
t.start()

# Attendre la fin
t.join()
print("Thread terminé")