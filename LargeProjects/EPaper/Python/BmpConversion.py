from PIL import Image

# Color mapping dictionary
color_mapping = {
    (0, 0, 0): 0b000,     # Black
    (255, 255, 255): 0b001,   # White
    (0, 255, 0): 0b010,   # Green
    (0, 0, 255): 0b011,   # Blue
    (255, 0, 0): 0b100,   # Red
    (255, 255, 0): 0b101,   # Yellow
    (255, 128, 0): 0b110   # Orange
}

# Function to convert RGB tuple to char based on color mapping
def rgb_to_char(rgb):
    return color_mapping.get(rgb, 0b001)  # Default to white if color not found

WIDTH = 600
HEIGHT = 448        
def sizeImage(img:Image) -> Image:        
    if img.width == WIDTH and img.height ==HEIGHT:
        pass
    elif img.width > WIDTH and img.height >HEIGHT:
        if WIDTH > img.height*HEIGHT/WIDTH:
            img = img.resize((int(img.width*HEIGHT/WIDTH),HEIGHT))
        else:
            img = img.resize((WIDTH, int(img.height*HEIGHT/WIDTH)))
    elif img.width < WIDTH and img.height <HEIGHT:
        if WIDTH > img.height*HEIGHT/WIDTH:
            img = img.resize((WIDTH, int(img.height*WIDTH/HEIGHT)))
        else:
            img = img.resize((int(img.width*WIDTH/HEIGHT),HEIGHT))        

    top = (img.height-HEIGHT)/2
    bot = img.height-top

    left = (img.width-WIDTH)/2
    right = img.width-left

    img = img.crop((left,top,right,bot))
    return img


# Function to read BMP image and convert to C++ array of chars
def bmp_to_cpp_array(image_path, output_path):
    # Open the image
    img = Image.open(image_path)
    img = sizeImage(img)
    # Convert image to RGB mode if not already in RGB
    img = img.convert("RGB")
    # Resize the image to ensure its width is even
    if img.width % 2 != 0:
        img = img.resize((img.width + 1, img.height))
    
    # Initialize the C++ array
    cpp_array = "const unsigned char testBitMap[] PROGMEM = {\n"
    
    # print("AAAAAAAAAA")
    # Iterate over each pixel pair
    for y in range(0, img.height, 1):
        for x in range(0, img.width, 2):
            # Get the RGB values of the 2x2 pixel block
            # print("Y = " + str(y))
            # print("X = " + str(x))
            pixel1 = img.getpixel((x,y))
            pixel2 = img.getpixel((x+1,y))
            # print(pixel1)
            # print(pixel2)
            pixels = [pixel1, pixel2]
            # print(pixels)
            # pixels = [img.getpixel((x + i, y )) for i in range(2) for j in range(2)]
            
            # Convert the RGB values to chars and combine them
            char_value = (rgb_to_char(pixels[0]) << 4) | rgb_to_char(pixels[1])
            # print(char_value)
            # # Append the char to the C++ array
            cpp_array += f"0x{char_value:02X},"
        cpp_array += '\n'
    # Remove the trailing comma and add the array closing bracket
    cpp_array = cpp_array.rstrip(",") + "\n};"
    
    # Write the C++ array to the output file
    with open(output_path, "w") as file:
        file.write(cpp_array)

# Function to read BMP image and convert to C++ array of chars
def bmp_to_raw_array(image_path, output_path):
    # Open the image
    img = Image.open(image_path)
    # img = sizeImage(img)
    # Convert image to RGB mode if not already in RGB
    img = img.convert("RGB")
    # Resize the image to ensure its width is even
    if img.width % 2 != 0:
        img = img.resize((img.width + 1, img.height))
    
    # Initialize the C++ array
    array = []
    
    # print("AAAAAAAAAA")
    # Iterate over each pixel pair
    for y in range(0, img.height, 1):
        for x in range(0, img.width, 2):
            # Get the RGB values of the 2x2 pixel block
            # print("Y = " + str(y))
            # print("X = " + str(x))
            pixel1 = img.getpixel((x,y))
            pixel2 = img.getpixel((x+1,y))
            # print(pixel1)
            # print(pixel2)
            pixels = [pixel1, pixel2]
            # print(pixels)
            # pixels = [img.getpixel((x + i, y )) for i in range(2) for j in range(2)]
            
            # Convert the RGB values to chars and combine them
            char_value = (rgb_to_char(pixels[0]) << 4) | rgb_to_char(pixels[1])
            # print(char_value)
            # # Append the char to the C++ array
            array.append(char_value)
    # Remove the trailing comma and add the array closing bracket
    # array = array.rstrip(",") 
    
    return array
    # Write the C++ array to the output file
    with open(output_path, "w") as file:
        file.write(array)

