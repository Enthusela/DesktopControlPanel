import socket
import traceback


def main():
    server_socket = get_server_socket()
    if server_socket is None:
        return
    handshake_messages(server_socket)
    listen_to_pi(server_socket)
    # non-existent function
    disconnect_from_pi(server_socket)


def get_server_socket():
    pi_address = "192.168.50.202"
    port = 5000
    print("Attempting to connect to server at " + pi_address + ":" + str(port) + "...")
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        server_socket.connect((pi_address, port))
    except Exception as e:
        print("Failed to bind socket to address.")
        traceback.print_exc()
        print("Exception message: " + str(e))
        return None
    return server_socket


def handshake_messages(server_socket):
    print("Handshaking with server...")
    try:
        server_socket.send("Hello, I'm Nathan's gaming PC.".encode())
        message = server_socket.recv(1024)
        print(message.decode())
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
    except Exception as e:
        print("Failed to send message to server.")
        traceback.print_exc()
        print("Exception message: " + str(e))


def listen_to_pi(server_socket):
    print("Listening to server...")
    while True:
        try:
            server_socket.send("I'm still listening.".encode())
            message = server_socket.recv(1024)
            print(message.decode())
        except KeyboardInterrupt:
            print("KeyboardInterrupt")
            break
        except Exception as e:
            traceback.print_exc()
            print("Exception message: " + str(e))
            break


def disconnect_from_pi(server_socket):
    print("Disconnecting from server...")
    try:
        server_socket.send("Goodbye.".encode())
        server_socket.close()
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
    except Exception as e:
        print("Failed to send message to server.")
        traceback.print_exc()
        print("Exception message: " + str(e))    


main()