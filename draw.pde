// Using Processing, lets a user easily draw an 8x8 pixel image. Prints the image to serial formatted to be copy-and-pasted into the Arduino IDE code.
// Note that this outputs the colors in strings with single quotes (') and must be converted to double quotes (") before running in Arduino IDE.
// Next steps: set up serial communication with "euclidean_sort_3_servo.ino" so there's no copy and pasting. ALT: make GUI on touchscreen instead of Processing.

// Pixel class definition
class Pixel {
  float x, y, w, h; // Position and dimensions
  String label;      // Pixel color
  color red, green, blue, yellow, black, white; // Colors for different states
  boolean isHovered = false;


  // Constructor
  Pixel(float x, float y, float w, float h) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.label = "white";
    this.red = color(230, 11, 11);
    this.green = color(121, 232, 93);
    this.blue = color(66, 125, 235);
    this.yellow = color(255, 242, 0);
    this.white = color(255, 255, 255);
    this.black = color(0, 0, 0);
  }

  // Draw the pixel
  void draw() {
    updateState();

    // Set pixel color based on state
    if (label == "white") {
      fill(white);
    } else if (label == "black") {
      fill(black);
    } else if (label == "red") {
      fill(red);
    } else if (label == "blue") {
      fill(blue);
    } else if (label == "green") {
      fill(green);
    } else if (label == "yellow") {
      fill(yellow);
    } else {
      fill(white);
    }

    // Draw pixel rectangle
    stroke(0);
    rect(x, y, w, h);

  }

  // Update the hover state
  void updateState() {
    isHovered = mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h;
  }

  // update pixel color on click
  void toggle() {
    if (isHovered && mousePressed) {
      label = brushColor;
    }
  }

  // Return pixel color state
  String getState() {
    return label;
  }
} 

// PaletteButton class definition
class PaletteWell {
  float x, y, w, h; // Position and dimensions
  String label;      // Color of the PaletteWell
  color red, green, blue, yellow, black, white; // Colors for different states
  boolean isHovered = false;


  // Constructor
  PaletteWell(float x, float y, float w, float h, String label) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.label = label;
    this.red = color(230, 11, 11);
    this.green = color(121, 232, 93);
    this.blue = color(66, 125, 235);
    this.yellow = color(255, 242, 0);
    this.white = color(255, 255, 255);
    this.black = color(0, 0, 0);
  }

  // Draw the button
  void draw() {
    updateState();

    // Set PaletteWell color based on state
    if (label == "white") {
      fill(white);
    } else if (label == "black") {
      fill(black);
    } else if (label == "red") {
      fill(red);
    } else if (label == "blue") {
      fill(blue);
    } else if (label == "green") {
      fill(green);
    } else if (label == "yellow") {
      fill(yellow);
    } else {
      fill(white);
    }

    // Draw palette well rectangle
    stroke(0);
    rect(x, y, w, h);

  }

  // Update the hover state
  void updateState() {
    isHovered = mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h;
  }

  // update brushcolor on click
  void toggle() {
    if (isHovered && mousePressed) {
      brushColor = label;
    }
  }

  // Return toggle state
  String getState() {
    return label;
  }
} 


// PrintButton class definition
class PrintButton {
  float x, y, w, h; // Position and dimensions
  String label;      // Text to display on the button
  color red, green, blue, yellow, black, white; // Colors for different states
  boolean isHovered = false;


  // Constructor
  PrintButton(float x, float y, float w, float h) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.label = "Print Image";
    this.red = color(230, 11, 11);
    this.green = color(121, 232, 93);
    this.blue = color(66, 125, 235);
    this.yellow = color(255, 242, 0);
    this.white = color(255, 255, 255);
    this.black = color(0, 0, 0);
  }

  // Draw the button
  void draw() {
    updateState();

    // Set button color based on state
    if (label == "white") {
      fill(white);
    } else if (label == "black") {
      fill(black);
    } else if (label == "red") {
      fill(red);
    } else if (label == "blue") {
      fill(blue);
    } else if (label == "green") {
      fill(green);
    } else if (label == "yellow") {
      fill(yellow);
    } else {
      fill(white);
    }

    // Draw button rectangle
    stroke(0);
    rect(x, y, w, h);
    
    // Draw button label
    fill(0);
    textAlign(CENTER, CENTER);
    text(label, x + w / 2, y + h / 2);

  }

  // Update the hover state
  void updateState() {
    isHovered = mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h;
  }

  // Toggle the button on click
  void toggle() {
    if (isHovered && mousePressed) {
      label = brushColor;
    }
  }

  // Return toggle state
  String getState() {
    return label;
  }
} 

// Main sketch
String brushColor = "black";
PaletteWell[] palette = new PaletteWell[6];
Pixel[] canvas = new Pixel[64];
PrintButton printButton;

void setup() {
  size(1000, 800);
  int palette_x = 850;
  palette[0] = new PaletteWell(palette_x, 100, 100, 100, "white");
  palette[1] = new PaletteWell(palette_x, 200, 100, 100, "black");
  palette[2] = new PaletteWell(palette_x, 300, 100, 100, "blue");
  palette[3] = new PaletteWell(palette_x, 400, 100, 100, "red");
  palette[4] = new PaletteWell(palette_x, 500, 100, 100, "green");
  palette[5] = new PaletteWell(palette_x, 600, 100, 100, "yellow");
  for (int y = 0; y < 8; ++y) {
    for (int x = 0; x < 8; ++x) {
      canvas[y+(x*8)] = new Pixel(x*100, y*100, 100, 100);
    }
  }
  printButton = new PrintButton(825, 710, 150, 50);
}

void draw() {
  background(240);
  for (int i = 0; i < 6; i+=1) {
    palette[i].draw();
  }
  for (int i = 0; i < 64; i+=1) {
    canvas[i].draw();
  }
  printButton.draw();
}

void mousePressed() {
  for (int i = 0; i < 64; ++i) {
    canvas[i].toggle();
  }

  if (printButton.isHovered && mousePressed) {
      print("{");
      for (int x = 0; x < 8; ++x) {
        print("{");
        for (int y = 0; y < 8; ++y) {
          print("'" + canvas[7-y+(x*8)].getState()+"'");
          if (y < 7){
            print(",");
          }
        }
        if (x < 7){
          print("},");
        } else{
          print("}");
        }
      }
      print("};");
    println();
  }
  
  for (int i = 0; i < 6; ++i) {
  if (palette[i].isHovered && mousePressed) {
      brushColor = palette[i].label;
    }
  }
}