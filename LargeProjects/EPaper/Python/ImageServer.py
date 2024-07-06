import socket
import random
import os
from PIL import Image

def getRandomImage():
    folder_path = './ConvertedImages'

    # List all files in the folder
    files = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]

    # Choose a random file
    random_file = random.choice(files)

    # Return the full path to the random file
    random_file_path = os.path.join(folder_path, random_file)
    return random_file_path


def generate_byte_array_from_bmp(image_path):
    """
    Generate a byte array from a BMP image where each pixel uses only 4 bits,
    resulting in each byte containing data for 2 pixels.

    Parameters:
    image_path (str): The path to the BMP image file.

    Returns:
    bytearray: A byte array containing the modified pixel data.
    int: The size of the resulting byte array.
    """
    # Open the BMP image
    print(image_path)
    image = Image.open(image_path)
    image = image.convert('L')  # Convert to grayscale

    # Get image dimensions
    width, height = image.size
    pixels = image.load()

    # Prepare the byte array
    byte_array = bytearray()

    # Iterate through the pixels and pack two pixels per byte
    for y in range(height):
        for x in range(0, width, 2):
            pixel1 = pixels[x, y] >> 4  # Use the upper 4 bits
            if x + 1 < width:
                pixel2 = pixels[x + 1, y] >> 4  # Use the upper 4 bits
            else:
                pixel2 = 0  # If width is odd, pad with zero for the last pixel
            combined_byte = (pixel1 << 4) | pixel2
            byte_array.append(combined_byte)

    # Print the size of the resulting byte array
    byte_array_size = len(byte_array)
    print(f"Size of the byte array: {byte_array_size} bytes")

    return byte_array, byte_array_size

def startImageServer(host='0.0.0.0', port=8001):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"Listening on {host}:{port}")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")
        binary_data, byte_array_size = generate_byte_array_from_bmp(getRandomImage())
        

        # Send the binary data
        client_socket.sendall(binary_data)
        
        client_socket.close()

# if __name__ == "__main__": 
    # Example binary data
    # there are 134400 bytes per image 
 

    # size = 134400
    # binary_data = bytearray(os.urandom(size)) #Random data
    # binary_data = bytearray([0x33] * size)
    # print(binary_data[-10:])
    
