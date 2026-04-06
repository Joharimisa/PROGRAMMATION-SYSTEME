import os
import sys

fd = int(sys.argv[1])
buffer = b""
while True:
    c = os.read(fd, 1)
    if c == b"\0" or c == b"":  # fin message
        break
    buffer += c
print(f"Fils : Message reçu : {buffer.decode()}")
os.close(fd)