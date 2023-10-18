import colorsys
import json
import re
import serial
import selectors
import socket
import struct
import time
import traceback

sel = selectors.DefaultSelector()
pc_data = {"VM": 1.0, "V1": 1.0, "V2": 1.0, "V3": 1.0}
argb_data = {"H": 96, "S": 255, "V": 128}


def static_vars(**kwargs):
    def decorate(func):
        for k in kwargs:
            setattr(func, k, kwargs[k])
        return func
    return decorate


def main():
    start_arduinos()
    start_server()
    print("Started listening for events.")

    while True:
        # TODO: Check for individual connection states and restart if necessary
        events = sel.select()
        for key, mask in events:
            callback = key.data
            callback(key.fileobj, mask)


def start_arduinos():
    start_arduino_uno()
    start_arduino_mega()


def start_arduino_uno():
    print("Starting uno...")
    try:
        uno = serial.Serial("/dev/ttyACM1", 9600, timeout=.1)
        # Wait for arduino to boot
        time.sleep(1)
        sel.register(uno, selectors.EVENT_WRITE, write_uno_data)
    except Exception as e:
        print("Uno serial connection error: " + str(e))
        return False


def start_arduino_mega():
    print("Starting mega...")
    try:
        mega = serial.Serial("/dev/ttyACM0", 9600, timeout=.1)
        # Wait for arduino to boot
        time.sleep(1)
        sel.register(mega, selectors.EVENT_READ, read_mega_data)
    except Exception as e:
        print("Mega serial connection error: " + str(e))
        return False


def start_server():
    print("Starting server...")
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(('0.0.0.0', 5000))
        sock.listen(5)
        sock.setblocking(False)
        sel.register(sock, selectors.EVENT_READ, accept)
    except OSError as e:
        print("Socket connection error: " + str(e))
        if sock:
            sock.close()
    except Exception as e:
        traceback.print_exc()
        print("Exception message: " + str(e))
    return False


def accept(sock, mask):
    pc_addr = '192.168.50.163'
    esp_addr = '192.168.50.241'
    print("Connection request received.")
    conn, addr = sock.accept()
    print(f"Connected to {addr}")
    ip_addr = addr[0]
    if ip_addr == pc_addr:
        print("PC connected.")
        sel.register(conn, selectors.EVENT_WRITE, write_PC_data)
    elif ip_addr == esp_addr:
        print("ESP connected.")
        sel.register(conn, selectors.EVENT_READ | selectors.EVENT_WRITE, handle_esp_data)
        

@static_vars(serial_msg_count=0)
def read_mega_data(conn, mask):
    commands = None
    command_start = "_"
    pc_cmds = ["A", "VM", "V1", "V2", "V3"]
    argb_cmds = ["H", "S", "V"]

    # Process serial data
    try:
        data = conn.readline().decode("utf-8").rstrip('\r\n')
        conn.flush()
        # print(data)
        # Inhibit errors for first two messages: these are usually truncated when the Mega boots
        if read_mega_data.serial_msg_count < 2:
            read_mega_data.serial_msg_count += 1
    except Exception as e:
        if read_mega_data.serial_msg_count > 2:
            print("Serial read error from Mega: " + str(e))
        return

    # Parse commands from serial data
    if data:
        # Commands are sent as one CSV string to avoid issues sending floats from Arduinos
        commands = re.split(",", data)

    if commands:
        if commands[0] != command_start:
            print("Warning: Invalid command start from Mega: " + commands[0])
            return
        for command in commands[1:]:
            # Only update values for valid commands: the arduino may drop and/or overlap some commands
            try:
                control, value = re.split(" ", command)
            except Exception as e:
                print("Warning: Invalid command from Mega: " + command + " (" + str(e) + ")")
                continue

            # Update values for valid commands
            if control in pc_cmds:
                pc_data[control] = value
            elif control in argb_cmds:
                argb_data[control] = value


def write_uno_data(conn, mask):
    try:
        conn.write('_'.encode())
        for value in argb_data.values():
            conn.write(struct.pack('B', int(value)))
    except Exception as e:
        print("Error: Serial write to Uno (" + str(e) + ")")


def write_PC_data(sock, mask):
    message = ""
    for control, value in pc_data.items():
        message += control + " " + str(value) + ","
    message.rstrip(",")
    message.rstrip("\n")
    try:
        sock.send(message.encode())
        return

    except BrokenPipeError:
        print("BrokenPipeError.")

    except Exception as e:
        print("Error: socket write to PC (" + str(e) + ")")

    sel.unregister(sock)
    print("PC socket unregistered.")


def handle_esp_data(sock, mask):
    if mask & selectors.EVENT_READ:
        read_esp_data(sock, mask)
    elif mask & selectors.EVENT_WRITE:
        write_esp_data(sock, mask)
    

def read_esp_data(sock, mask):
    try:
        data = sock.recv(1024) 
        if data:
            # print(data, end='\n')
            return

    except BrokenPipeError:
        print("BrokenPipeError.")

    except Exception as e:
        print("Error: socket read from ESP (" + str(e) + ")")

    sel.unregister(sock)
    print("ESP socket unregistered.")


def write_esp_data(sock, mask):
    try:
        # convert HSV values to RGB
        # h = float(argb_data["H"])
        # s = float(argb_data["S"])
        # v = float(argb_data["V"])
        # print("hsv: " + str(h) + ", " + str(s) + ", " + str(v))
        # r, g, b = colorsys.hsv_to_rgb(h, s, v)
        # print("rgb: " + str(r) + ", " + str(g) + ", " + str(b))

        esp_data = {}
        esp_data["r"] = 255; # int(argb_data["H"])
        esp_data["g"] = 255; # int(argb_data["S"])
        esp_data["b"] = 255; # int(argb_data["V"])
        esp_data["w"] = 255; # int((int(pc_data["VM"]) - 1) / 4)

        # Send data to esp as JSON
        message = json.dumps(esp_data) + '\n'
        # print("esp message: ", end="")
        # print(message)
        sock.sendall(message.encode())
        return

    except BrokenPipeError:
        print("BrokenPipeError.")

    except Exception as e:
        print("Error: socket write to ESP (" + str(e) + ")")

    sel.unregister(sock)
    print("ESP socket unregistered.")


def disconnect_from_PC(client_socket):
    print("Disconnecting from PC...")
    client_socket.close()


def prct_from_10bit(value):
    return round(float(value) / 1023.0 * 1.0, 2)


main()
