import socket
import random
import os
from BmpConversion import bmp_to_raw_array

def getRandomImage():
    folder_path = './ConvertedImages'

    # List all files in the folder
    files = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]

    # Choose a random file
    random_file = random.choice(files)

    # Return the full path to the random file
    random_file_path = os.path.join(folder_path, random_file)
    print(random_file_path)
    return random_file_path



def startImageServer(host='0.0.0.0', port=8001):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"Listening on {host}:{port}")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")
        binary_data = bmp_to_raw_array(getRandomImage())
        

        # Send the binary data
        client_socket.sendall(binary_data)
        
        client_socket.close()
        return

# if __name__ == "__main__": 
    # Example binary data
    # there are 134400 bytes per image 
 

    # size = 134400
    # binary_data = bytearray(os.urandom(size)) #Random data
    # binary_data = bytearray([0x33] * size)
    # print(binary_data[-10:])
    
