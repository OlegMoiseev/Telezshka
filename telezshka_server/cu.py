import socket


def create_conn(ip_in: str, port_in: int):
    sock = socket.socket()
    sock.connect((ip_in, port_in))
    return sock


ip = "192.168.1.100"
port = 9093
to_server_sock = create_conn(ip, port)


while True:

    in_str = input("Send to server:")
    to_server_sock.send(in_str.encode())

    while True:
        answer = to_server_sock.recv(128).decode()
        print("Received:", answer)
        if '-1.0 -1.0 -1.0 -1.0 -1.0 -1.0' in answer:
            break

    if answer == 'stop':
        break

to_server_sock.close()
# 1 30.0 150.0 500.0 30.0 150.0 500.0 30.0 150.0 500.0
# 1 30.0 -150.0 500.0 30.0 -150.0 500.0 30.0 -150.0 500.0
