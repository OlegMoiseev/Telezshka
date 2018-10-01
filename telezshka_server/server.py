import serial
import socket
import time


def create_server(ip_in: str, port_in: int):
    sock = socket.socket()
    sock.bind((ip_in, port_in))
    sock.listen(1)
    return sock


def validate_str(in_str: str):
    parsed = in_str.split()
    if len(parsed) == 9:
        return True
    else:
        return False


def report_done(sock):
    sock.send("done".encode())


def report_error(num, sock):
    sock.send((num + " error").encode())


def report_invalid(sock):
    sock.send("invalid string".encode())


class Telega:
    def __init__(self, com_port: str):
        self.com = serial.Serial(com_port, baudrate=9600, timeout=120)
        # pass

    def send_to(self, in_str: str):
        self.com.write(in_str.encode())
        print("Send to telega:", in_str.encode())

    def recv_from(self):
        ans = self.com.read(4).decode()
        print("Recv from telega:", ans.encode())
        return ans
        # return 0

    def recv_init(self):
        ans = self.com.readline().decode()
        print("Recv from telega:", ans.encode())
        return ans

    def stop(self):
        self.com.close()
        # pass


def two_command_move(sock, obj):
    rot_angle = sock.recv(128).decode()
    print("From CU: ", rot_angle)
    go_line = sock.recv(128).decode()
    print("From CU: ", go_line)
    if rot_angle == zero_str and go_line == zero_str:
        report_done(sock)

    if validate_str(rot_angle) and validate_str(go_line):
        obj.send_to(rot_angle)

        ans = obj.recv_from()

        if "done" in ans:

            obj.send_to(go_line)

            ans = obj.recv_from()

            if "done" in ans:
                report_done(sock)
            else:
                report_error("2", sock)
                return 1
        else:
            report_error("1", sock)
            return 1
    else:
        report_invalid(sock)
        return 2

    return 0


def one_command_move(sock, obj):
    go_line = sock.recv(128).decode()
    print("From CU: ", go_line)
    if go_line == zero_str:
        report_done(sock)

    if validate_str(go_line):
        obj.send_to(go_line)

        ans = obj.recv_from()

        if "done" in ans:

            report_done(sock)

        else:
            report_error("1", sock)
            return 1
    else:
        report_invalid(sock)
        return 2

    return 0


telega = Telega("/dev/ttyUSB0")  # from Arduino studio COM-port
zero_str = "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0"
ip = ""  # empty == localhost
port = 9092

print("Server started!")
from_cu_sock, addr_from = create_server(ip, port).accept()
print("Connected CU by", port, ":", addr_from)
print(telega.recv_init())
while True:
    if one_command_move(from_cu_sock, telega) != 0:
        break

from_cu_sock.close()
telega.stop()
