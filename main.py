from PIL import Image  # Import the Python Imaging Library for image processing
import numpy as np  # Import NumPy for numerical operations
import serial  # Import PySerial for serial communication
import json  # Import JSON for serializing and deserializing data

# Predefined RGB values for specific colors
color = {
    "green": (91, 173, 62),
    "red": (193, 6, 21),
    "yellow": (233, 214, 32),
    "blue": (20, 71, 167),
    "black": (15, 15, 15),
    "white": (240, 240, 240)
}

# Open the image file
im = Image.open("images/rainbow.jpg")
# Resize the image to 8x8 pixels for a smaller, fixed size
im = im.resize((8, 8))

# Initialize an empty list to store color names corresponding to pixels
colors = []

# Iterate over all the pixels in the image
for pixel in list(im.getdata()):
    # Initialize a variable to store the closest color (distance, color name)
    closest_color = (999, "")  # Start with a large distance
    for rgb in color:  # Iterate over the predefined colors
        # Calculate the Euclidean distance between the current pixel and the predefined color
        dist = np.linalg.norm(np.subtract(pixel, color[rgb]))
        # Update the closest color if a smaller distance is found
        if dist < closest_color[0]:
            closest_color = (dist, rgb)
    # Append the name of the closest color to the colors list
    colors.append(closest_color[1])

# Convert the list of color names to a NumPy array
colors = np.array(colors)
# Reshape the array into an 8x8 matrix
colors = colors.reshape((8, 8))

# Set up the serial communication with the specified port and baud rate
ser = serial.Serial("COM7", 9600)

# Convert the 8x8 matrix into a JSON string for transmission
matrix_string = json.dumps(colors)

# Send a start message to indicate data transmission is beginning
ser.write(bytearray("Sending Data", "ascii"))
# Send the serialized matrix string
ser.write(matrix_string.encode('utf-8'))
# Send a completion message to indicate all data has been sent
ser.write(bytearray("Done Sending Data", "ascii"))

# Close the serial connection
ser.close
