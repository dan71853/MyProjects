import socket

def start_server(host='0.0.0.0', port=8001, data=b''):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"Listening on {host}:{port}")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")

        # Send the binary data
        client_socket.sendall(data)
        
        client_socket.close()

if __name__ == "__main__":
    # Example binary data
    binary_data = b'\x01\x02\x03\x04\x05'
    start_server(data=binary_data)
