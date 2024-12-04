from PIL import Image  # Import the Python Imaging Library for image processing
import numpy as np  # Import NumPy for numerical operations
import serial  # Import PySerial for serial communication
import json  # Import JSON for serializing and deserializing data
import time

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
im = Image.open("images/rainbow.jpg").convert("RGB")
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
try:
    ser = serial.Serial("/dev/cu.usbmodem1301", 9600)
    time.sleep(2)
    print("connected")

    # Send a start message to indicate data transmission is beginning
    # ser.write(bytearray("Sending Data", "ascii"))
    # print("sending data")
    # # Send the serialized matrix string
    # ser.write(matrix_string.encode('utf-8'))
    # print("sending string")
    # # Send a completion message to indicate all data has been sent
    # ser.write(bytearray("Done Sending Data", "ascii"))
    # print("completing handshake")

    color_map = {
        "green": 0,
        "red": 1,
        "yellow": 2,
        "blue": 3,
        "black": 4,
        "white": 5
    }

    # Replace color names with indices in the matrix
    
    color_indices = np.vectorize(color_map.get)(colors)

    matrix_string = json.dumps(color_indices.tolist())

    ser.write(matrix_string.encode('utf-8'))
    ser.write(b"\n")  # Newline to signify end of data

    while True:
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8').strip()
            print("Arduino:", response)
            if response == "Done receiving and processing data":
                break

    # Close the serial connection
    ser.close()
    print("closing connection")
except:
    print("cant open serial channel")

# Create a blank image of size 8x8
visualization = Image.new("RGB", (8, 8))

# Fill the image with the corresponding RGB values
for y in range(8):
    for x in range(8):
        # Get the color name from the colors array and map it to its RGB value
        visualization.putpixel((x, y), color[colors[y, x]])

# Save the image as a PNG
visualization.save("output_visualization.png")
print("Visualization saved as output_visualization.png")