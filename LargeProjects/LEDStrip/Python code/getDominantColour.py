import pyscreenshot as ImageGrab
from PIL import Image
import time
import serial
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
    # Grab a screenshot of the entire screen
    im = ImageGrab.grab()


    # Crop the screenshot to the rectangle
    im_crop = im.crop((rect_left, rect_top, rect_right, rect_bottom))

    # Get the most common color in the cropped image
    colors = im_crop.getcolors(im_crop.size[0] * im_crop.size[1])
    most_common_color = max(colors, key=lambda x: x[0])[1]

    # pack RGB values and trailing byte into a byte string
    data = bytes([0xff,most_common_color[0], most_common_color[1], most_common_color[2]])

    # send data to serial port
    ser.write(data)

    # wait for 500ms before sending next data
    time.sleep(0.2)
