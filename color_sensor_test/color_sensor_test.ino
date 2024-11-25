#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("found sensor");
  } else {
    Serial.println("no sensor found");
    while (1)
      ;  // end program
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  //check for significant color
  int threshold = 50;
  if (c > threshold) {
    //normalize values
    float sum = c;
    float red = r / sum;
    float green = g / sum;
    float blue = b / sum;
    
    Serial.print("R: "); Serial.print(r); Serial.print(" ");
    Serial.print("G: "); Serial.print(g); Serial.print(" ");
    Serial.print("B: "); Serial.print(b); Serial.print(" ");
    Serial.print("C: "); Serial.print(c); Serial.print(" ");

    //calc colorTemp and lux
    uint16_t colorTemp = tcs.calculateColorTemperature(r, g, b);
    uint16_t lux = tcs.calculateLux(r, g, b);

    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K -");
    Serial.print("Lux: "); Serial.println(lux, DEC);

    // direct against led

    if (r+g+b < 100){ //black
      Serial.println("black");
    } else if (r+g+b > 10000){ //white
      Serial.println("white");
    } else if (r > 1000 && g < 700 && b < 700){ //red
      Serial.println("red");
    } else if (g*1.5 > r+b && r+g+b < 5000){ //green
      Serial.println("green");
    } else if (r+g+b > 500 && b > r && b > g){ //blue
      Serial.println("blue");
    } else if (5000 < r+g+b < 10000 && b*4 < r+g){ //yellow
      Serial.println("yellow");
    } else {
      Serial.println("idk color");
    }
    


    if (c > 850){
      if (g+b > 850){ //white
        Serial.println("white");
      } else { // yellow
        Serial.println("yellow");
      }
    } else {
      if (c < 400 && c> 300){ // black
        Serial.println("black");
      } else if (g>b*1.5 && g > 200){
        Serial.println("green");
      } else if (r>g*1.5 && r > 200){
        Serial.println("red");
      } else if (r<b && b<g){
        Serial.println("blue");
      } else{
        Serial.println("dunno");
      }
    }


    
  }

  else {
    Serial.println("no significant color");
  }
  

}
