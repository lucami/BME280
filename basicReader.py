import socket
import datetime

HOST = "0.0.0.0"
PORT = 6660

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))

print(f"In ascolto su UDP {HOST}:{PORT}...")

while True:
    data, addr = sock.recvfrom(65535)
    message = data.decode("utf-8", errors="replace").lstrip()
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
    print(f"[{timestamp}] {message}")
