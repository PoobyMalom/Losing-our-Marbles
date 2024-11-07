// color sorting script
// future improvements: add safety to ensure solenoid isn't on for more than 1s
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);



// initialize variables
int DROPPER_SOLENOID_PIN = 12;
int DROPPER_SERVO_PIN = 5;
int BLACK_COL_ANG = 45; // servo angle for black column (no gear ratios)
int WHITE_COL_ANG = 80; // servo angle for white column (no gear ratios)
int OUT_COL_ANG = 0; // servo angle for marble disposal
int SOLENOID_ON_TIME = 100;

String marbleColor;

Servo myservo;  // create Servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup(){   
  pinMode(DROPPER_SOLENOID_PIN,OUTPUT); // RELAY PIN   
  digitalWrite(DROPPER_SOLENOID_PIN,HIGH); // Normally ON Only For Chanies Relay Module   
  myservo.attach(DROPPER_SERVO_PIN);  // attaches the servo on pin 5 to the Servo object

  Serial.begin(9600);

  if (tcs.begin()){
    Serial.println("found sensor");
  } else {
    Serial.println("no sensor found");
    while (1); // end program
  }
}  

void loop(){

  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  //check for significant color
  int threshold = 50;
  if (c > threshold){
    //normalize values
    float sum = c;
    float red = r/ sum;
    float green = g/ sum;
    float blue = b/ sum;

    Serial.print("R: "); Serial.print(r); Serial.print(" ");
    Serial.print("G: "); Serial.print(g); Serial.print(" ");
    Serial.print("B: "); Serial.print(b); Serial.print(" ");
    Serial.print("C: "); Serial.print(c); Serial.print(" ");


    if (c > 850){
      if (g+b > 850){ //white
        Serial.println("white");
        marbleColor = "white";
      } else { // yellow
        Serial.println("yellow");
        marbleColor = "yellow";
      }
    } else {
      if (c < 400 && c> 300){ // black
        Serial.println("black");
        marbleColor = "black";
      } else if (g>b*1.5 && g > 200){
        Serial.println("green");
        marbleColor = "green";
      } else if (r>g*1.5 && r > 200){
        Serial.println("red");
        marbleColor = "red";
      } else if (r<b && b<g){
        Serial.println("blue");
        marbleColor = "blue";
      } else{
        Serial.println("dunno");
        marbleColor = "dunno";
      }
    }
  }

  else {
    Serial.println("no significant color");
  }
  digitalWrite(DROPPER_SOLENOID_PIN,LOW); // RELAY ON. DONT POWER FOR MORE THAN 1 SEC
  // color sensors determines color of marble. create "marbleColour"
  if (marbleColor == "black"){
    myservo.write(BLACK_COL_ANG);
    delay(1000);
  }
  else if (marbleColor == "white"){
    myservo.write(WHITE_COL_ANG);
    delay(1000);
  }
  else {
    myservo.write(OUT_COL_ANG);
    delay(1000);
  }
  digitalWrite(DROPPER_SOLENOID_PIN,HIGH); // RELAY OFF
  
}


