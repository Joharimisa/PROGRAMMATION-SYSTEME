import sys

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} nombre1 nombre2")
    sys.exit(1)

a = int(sys.argv[1])
b = int(sys.argv[2])

somme = a + b

print(f"La somme de {a} et {b} est {somme}")