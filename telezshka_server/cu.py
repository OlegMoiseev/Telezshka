import socket


def create_conn(ip_in: str, port_in: int):
    sock = socket.socket()
    sock.connect((ip_in, port_in))
    return sock


ip = ""
port = 9092
to_server_sock = create_conn(ip, port)


while True:
    in_str = input("Send to server:")
    to_server_sock.send(in_str.encode())
    in_str = input("Send to server:")
    to_server_sock.send(in_str.encode())
    
    answer = to_server_sock.recv(128).decode()
    print("Received:", answer)
    if answer == 'stop':
        break

to_server_sock.close()
