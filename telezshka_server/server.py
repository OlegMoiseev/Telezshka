import json
import serial
import socket
import time


class Server:
    def __init__(self, ip_server: str, port_server: int):
        self.zero_str = "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0"
        self.sock = socket.socket()
        self.sock.bind((ip_server, port_server))
        self.sock.listen(1)

        self.done_msg = "done"
        self.interruption = "1\r\n"

        print("Server started!")

    def create_con(self):
        self.connection, self.addr_from = self.sock.accept()
        print("Connected CU by", self.connection, ":", self.addr_from)

    def report_done(self):
        self.connection.send("0.0 0.0 0.0 0.0 0.0 0.0".encode())

    def report_error(self, num):
        self.connection.send((num + " error").encode())

    def report_invalid(self):
        self.connection.send("-1".encode())

    def report_info(self, data):
        self.connection.send(data.encode())

    def two_command_move(self, obj):
        rot_angle = self.connection.recv(128).decode()
        print("From CU: ", rot_angle)
        go_line = self.connection.recv(128).decode()
        print("From CU: ", go_line)

        if rot_angle == self.zero_str and go_line == self.zero_str:
            self.report_done()

        if obj.validate_str(rot_angle) and obj.validate_str(go_line):
            obj.send_to(rot_angle)

            ans = obj.recv_from()

            if "done" in ans:

                obj.send_to(go_line)

                ans = obj.recv_from()

                if "done" in ans:
                    self.report_done()
                else:
                    self.report_error("2")
                    return 1
            else:
                self.report_error("1")
                return 1
        else:
            self.report_invalid()
            return 2

        return 0

    def one_command_move(self, obj):
        go_line = self.connection.recv(128).decode()[2:]
        print("From CU: ", go_line)
        # if go_line == self.zero_str:
        #     self.report_done()

        if obj.validate_str(go_line):
            obj.send_to(go_line)
            # -----------------------------
            # CRUTCH
            go = go_line.split()
            sign_negative = [False, False, False]
            sign_negative[0] = True if go[1][0] == '-' else False
            sign_negative[1] = True if go[4][0] == '-' else False
            sign_negative[2] = True if go[7][0] == '-' else False
            # -----------------------------

            tracking = True
            old_ans = ""
            while tracking:
                ans = obj.recv_from()
                if self.done_msg in ans:
                    tracking = False
                    old_ans += "1 "
                    self.report_info(old_ans)

                elif ans == self.interruption:
                    tracking = False
                    old_ans += "2 "
                    self.report_info(old_ans)

                else:
                    # -----------------------------
                    # CRUTCH
                    tmp = ans.split()

                    if sign_negative[0]:
                        tmp[0] = -1 * float(tmp[0])
                    if sign_negative[1]:
                        tmp[2] = -1 * float(tmp[2])
                    if sign_negative[2]:
                        tmp[4] = -1 * float(tmp[4])

                    ans = ""
                    for i in range(len(tmp)):
                        ans += str(tmp[i]) + " "
                    old_ans = ans
                    ans += "0 "
                    print(ans)
                    # -----------------------------
                    self.report_info(ans)

        else:
            self.report_invalid()
            return 2

        return 0

    def stop(self):
        self.connection.close()

    def start(self, obj):
        try:
            obj.change_timeout(120)
            while True:
                if self.one_command_move(obj) != 0:
                    break
        except Exception as e:
            obj.change_timeout(5)
            while True:
                ans = telega.recv_from()
                print("Skipped:", ans)
                if " 1 " in ans or " 2 " in ans or ans == b'':
                    break
                else:
                    pass
            print(e)
            print("Connect aborted by exception")


class Telega:
    def __init__(self, com_port: str):
        self.com = serial.Serial(com_port, baudrate=115200, timeout=120)
        # pass

    def change_timeout(self, sec):
        self.com.timeout = sec

    def send_to(self, in_str: str):
        self.com.write(in_str.encode())
        # print("Send to telega:", in_str.encode())

    def recv_from(self):
        ans = self.com.readline().decode()
        print("Recv from telega:", ans.encode())
        return ans

    def recv_init(self):
        ans = self.com.readline()
        return ans

    def stop(self):
        self.com.close()
        # pass

    def validate_str(self, in_str: str):
        parsed = in_str.split()
        return len(parsed) == 9

    def reset_buffers(self):
        self.com.reset_input_buffer()
        self.com.reset_output_buffer()


with open("./config.json") as file:
    config_json = json.load(file)

telega = Telega(config_json["RobotServer"]["COMPort"])  # from Arduino studio COM-port
print(telega.recv_init())
print("Telega started!")
ip = config_json["RobotServer"]["IPAddress"]  # empty == localhost
port = int(config_json["RobotServer"]["port"])

print("Application started!")

server_for_cu = Server(ip, port)

default_timeout = 120
skip_timeout = 5
while True:
    try:
        telega.reset_buffers()
        server_for_cu.create_con()
        server_for_cu.start(telega)

        server_for_cu.stop()
        print("Connection aborted by Danya")
        # time.sleep(5)
    except Exception as e:

        server_for_cu.stop()
        print(e)
        print("except")
telega.stop()
