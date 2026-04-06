from multiprocessing import Process, Pipe

def fils(pipe):
    mot = input("Fils : Entrez un mot : ")
    pipe.send(mot)
    result = pipe.recv()
    print(f"Fils : Le mot {'est' if result else \"n'est pas\"} dans le fichier.")

def pere(pipe):
    mot = pipe.recv()
    found = False
    with open("fichier.txt") as f:
        for word in f.read().split():
            if word == mot:
                found = True
                break
    pipe.send(found)

if __name__ == "__main__":
    parent_conn, child_conn = Pipe()
    p = Process(target=fils, args=(child_conn,))
    p.start()
    pere(parent_conn)
    p.join()