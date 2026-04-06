import sys
import os

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} n k")
    sys.exit(1)

n = int(sys.argv[1])
k = int(sys.argv[2])

# Cas de base
if k == 0 or k == n:
    print(f"C({n},{k}) = 1")
    sys.exit(0)

file1 = f"/tmp/C_{n-1}_{k}.tmp"
file2 = f"/tmp/C_{n-1}_{k-1}.tmp"

# Appel du programme lui-même pour simuler récursion
os.system(f"python3 {sys.argv[0]} {n-1} {k} > {file1}")
os.system(f"python3 {sys.argv[0]} {n-1} {k-1} > {file2}")

# Lire les valeurs intermédiaires
with open(file1) as f:
    val1 = int(f.read().split('=')[1])
with open(file2) as f:
    val2 = int(f.read().split('=')[1])

os.remove(file1)
os.remove(file2)

print(f"C({n},{k}) = {val1 + val2}")