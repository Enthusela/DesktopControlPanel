import socket
import traceback

def main():
    server_socket = start_server()
    client_socket, client_address = wait_for_PC_connection(server_socket)
    if client_socket and client_address:
        handshake_messages(client_socket)
        send_user_message(client_socket)
        disconnect_from_PC(client_socket)

def start_server():
    print("Starting server...")
    port = 5000
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        server_socket.bind(("0.0.0.0", port))
        server_socket.listen(5)
        return server_socket
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
    except Exception as e:
        traceback.print_exc()
        print("Exception message: " + str(e))
    return False

def wait_for_PC_connection(server_socket):
    print("Waiting for PC to connect...")
    while True:
        try:
            client_socket, client_address = server_socket.accept()
            print(f"Connected to {client_address}")
            return client_socket, client_address
        except KeyboardInterrupt:
            print("KeyboardInterrupt")
        except Exception as e:
            print(f"No connection made")
            traceback.print_exc()
        return False

def handshake_messages(client_socket):
    print("Handshaking with PC...")
    try:
        message = client_socket.recv(1024)
        client_socket.send("Hello, I'm the Raspberry Pi server.".encode())
        print(message.decode())
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
    except Exception as e:
        traceback.print_exc()
        print("Exception message: " + str(e))

def send_user_message(client_socket):
    while True:
        message = input("Enter message to PC: ")
        try:
            client_socket.send(message.encode())
        except KeyboardInterrupt:
            print("KeyboardInterrupt")
            break
        except Exception as e:
            print("Failed to send message.")
            traceback.print_exc()
            print("Exception message: " + str(e))
            break

        print(f"Sent message '{message}'.")

def disconnect_from_PC(client_socket):
    print("Disconnecting from PC...")
    client_socket.close()

main()