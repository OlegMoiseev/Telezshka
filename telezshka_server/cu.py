import socket
import time

def create_conn(ip_in: str, port_in: int):
    sock = socket.socket()
    sock.connect((ip_in, port_in))
    return sock


ip = "192.168.1.100"
port = 9093
to_server_sock = create_conn(ip, port)

forward = True
num_of_travels = 0


while True:
    # if forward:
    #     in_str = "1 0.0 -200.0 1000.0 0.0 -200.0 1000.0 0.0 -200.0 1000.0"
    # else:
    #     num_of_travels += 1
    #     print(num_of_travels)
    #     in_str = "1 0.0 200.0 1000.0 0.0 200.0 1000.0 0.0 200.0 1000.0"

    in_str = input("Send to server:")
    to_server_sock.send(in_str.encode())
    if in_str == "STOP":
        break

    while True:
        answer = to_server_sock.recv(128).decode()
        print("Received:", answer)
        if " 1 " in answer or " 2 " in answer:
            break
    # forward = not forward
    #
    # time.sleep(1)

to_server_sock.close()

# 1 30.0 200.0 500.0 30.0 200.0 500.0 30.0 200.0 500.0
# 1 30.0 -200.0 400.0 30.0 -200.0 400.0 30.0 -200.0 400.0
# 1 0.0 -200.0 400.0 0.0 -200.0 400.0 0.0 -200.0 400.0
# 1 0.0 200.0 400.0 0.0 200.0 400.0 0.0 200.0 400.0
