import numpy as np
from PIL import ImageGrab

# Define rectangle dimensions as half of screen size
screen_size = ImageGrab.grab().size
rect_size = (screen_size[0] // 2, screen_size[1] // 2)
rect_left = (screen_size[0] // 4, screen_size[1] // 4)
rect_right = (rect_left[0] + rect_size[0], rect_left[1] + rect_size[1])

# Grab screen image and extract rectangle
screen = np.array(ImageGrab.grab())
rect = screen[rect_left[1]:rect_right[1], rect_left[0]:rect_right[0]]

# Calculate weights for weighted average
weights = np.zeros(rect_size)
x, y = np.meshgrid(np.arange(rect_size[0]), np.arange(rect_size[1]))
weights += np.exp(-0.5 * ((x - rect_size[0] / 2) / (rect_size[0] / 6)) ** 2)
weights += np.exp(-0.5 * ((y - rect_size[1] / 2) / (rect_size[1] / 6)) ** 2)
weights /= np.sum(weights)

# Calculate weighted average color
average_color = np.sum(rect * np.dstack([weights, weights, weights]), axis=(0, 1)).astype(np.uint8)

print(f"Average color: ({average_color[0]}, {average_color[1]}, {average_color[2]})")
