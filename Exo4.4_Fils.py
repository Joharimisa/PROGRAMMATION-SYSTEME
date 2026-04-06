import os

fd_str = os.environ.get("FD_TUBE")
if fd_str is None:
    print("FD_TUBE non défini")
    exit(1)

fd = int(fd_str)
buffer = b""
while True:
    c = os.read(fd, 1)
    if c == b"\0" or c == b"":  # fin message
        break
    buffer += c

print(f"Fils : Message reçu : {buffer.decode()}")
os.close(fd)