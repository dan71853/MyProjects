import random
import time
import serial
import PIL.ImageGrab as ImageGrab
import numpy as np

# configure serial port
ser = serial.Serial('COM3', 9600)  # replace 'COM3' with your serial port name

# Define the dimensions of the rectangle
screen_width, screen_height = ImageGrab.grab().size
rect_width, rect_height = screen_width // 2, screen_height // 2
rect_left = (screen_width - rect_width) // 2
rect_top = (screen_height - rect_height) // 2
rect_right = rect_left + rect_width
rect_bottom = rect_top + rect_height



while True:


    # start_time = time.time()

    # Capture the screenshot and crop the rectangle
    screenshot = ImageGrab.grab()
    cropped_screenshot = screenshot.crop((rect_left, rect_top, rect_right, rect_bottom))

    # Convert the cropped screenshot to a NumPy array
    np_array = np.array(cropped_screenshot)

    # Calculate the average color of the rectangle
    avg_color = np.mean(np_array, axis=(0, 1)).astype(int)

   
    
    

    # pack RGB values and trailing byte into a byte string
    data = bytes([avg_color[0], avg_color[1], avg_color[2], 0xff])
    
    # send data to serial port
    ser.write(data)
    
    # wait for 500ms before sending next data
    time.sleep(0.2)
 