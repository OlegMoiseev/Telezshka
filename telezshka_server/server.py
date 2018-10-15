import json
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
    sock.send("1 ".encode())


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
        # print("Send to telega:", in_str.encode())

    def recv_from(self):
        # time.sleep(1)
        ans = self.com.read(4).decode()
        # ans = "done"
        print("Recv from telega:", ans.encode())
        return ans

    def recv_init(self):
        ans = self.com.readline().decode()
        # ans = "Init completed"
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
    go_line = sock.recv(128).decode()[2:]
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


with open("config.json") as file:
    config_json = json.load(file)

telega = Telega(config_json["RobotServer"]["COMPort"])  # from Arduino studio COM-port
print(telega.recv_init())
print("Telega started!")
ip = config_json["RobotServer"]["IPAddress"]  # empty == localhost
port = int(config_json["RobotServer"]["port"])
zero_str = "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0"

print("Application started!")

while True:
    from_cu_sock, addr_from = create_server(ip, port).accept()
    print("Server started!")
    print("Connected CU by", port, ":", addr_from)
    try:
        while True:
            if one_command_move(from_cu_sock, telega) != 0:
                break

    except:
        print("Connect aborted by exception")

    from_cu_sock.close()
    print("Connection aborted by Danya")
    time.sleep(5)

telega.stop()
