// color sorting with euclidean distance for sprint 1 mechanical system
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
String marbleColor;
float cMultiplier = 0.02;
Servo myservo;  // create Servo object to control a servo
int pos = 0;    // variable to store the servo position

// initialize variables
int DROPPER_SOLENOID_PIN = 12;
int DROPPER_SERVO_PIN = 5;
int COL_ONE_ANG = 45;  // servo angle for black column (no gear ratios)
int COL_TWO_ANG = 80;  // servo angle for white column (no gear ratios)
int COL_OUT_ANG = 0;   // servo angle for marble disposal


float colorVectors[7][4] = {
  //rgbc values for the different marble colors
  { 4867, 5387, 3507, 13291 },  // white
  {222, 249, 169, 616},      // black
  {1558, 538, 407, 2259},     // red
  {1063, 1573, 689, 3255},    // green
  {417, 558, 593, 1473},     // blue
  { 3239, 2906, 1103, 7071 },   // yellow
  {76, 69, 51, 189}          // nothing
};

char *colorLabels[] = { "white", "black", "red", "green", "blue", "yellow", "nothing" };
unsigned long previousMillis = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(DROPPER_SOLENOID_PIN,OUTPUT); // RELAY PIN   
  digitalWrite(DROPPER_SOLENOID_PIN,LOW); // Normally ON Only For Chanies Relay Module   
  myservo.attach(DROPPER_SERVO_PIN);  // attaches the servo on pin 5 to the Servo object

  Serial.begin(9600);
  pinMode(DROPPER_SOLENOID_PIN, OUTPUT);     // RELAY PIN
  digitalWrite(DROPPER_SOLENOID_PIN, HIGH);  // Normally ON Only For Chanies Relay Module
  myservo.attach(DROPPER_SERVO_PIN);         // attaches the servo on pin 5 to the Servo object

  if (tcs.begin()) {
    Serial.println("found sensor");
  } else {
    Serial.println("no sensor found");
    while (1)
      ;  // end program
  }
}

float euclideanDistance(float *marbleVec, float *refVec) {
  // give two 4d vectors and it spits out the euclidean distance of them
  return sqrt(pow(marbleVec[0] - refVec[0], 2) + pow(marbleVec[1] - refVec[1], 2) + pow(marbleVec[2] - refVec[2], 2) + pow((marbleVec[3] - refVec[3]) * cMultiplier, 2));
}
int minValue(float *myArray) {
  // give it an array and it spits out the index of the smallest value in the array
  int minValIndex = 0;
  float minVal = myArray[0];

  for (int i = 0; i < 7; i++) {
    if (myArray[i] < minVal) {
      minVal = myArray[i];
      minValIndex = i;
    }
  }
  return minValIndex;
}

void loop() {
  unsigned long currentMillis = millis();
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  float sum = float(c) * cMultiplier;
  float red = r * 100 / float(c);
  float green = g * 100 / float(c);
  float blue = b * 100 / float(c);
  float marbleVector[4] = { r, g, b, c };

  Serial.print("raw marble values ");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.print(b);
  Serial.print(", ");
  Serial.println(c);

  // find euclidean distance of rgb of this marble to the reference for every marble color
  float allDistances[7];
  for (int i = 0; i < 7; i++) {
    allDistances[i] = euclideanDistance(marbleVector, colorVectors[i]);
    Serial.print(allDistances[i]);
    Serial.print(", ");
  }
  Serial.println(" ");


  // find closest distance (scale down c basis vector because its less important)
  int index_minimum_value = minValue(allDistances);

  // find marble color from closest distance
  marbleColor = colorLabels[index_minimum_value];
  Serial.print(marbleColor);
  Serial.print(": ");
  Serial.print(colorVectors[index_minimum_value][0]);
  Serial.print(", ");
  Serial.print(colorVectors[index_minimum_value][1]);
  Serial.print(", ");
  Serial.print(colorVectors[index_minimum_value][2]);
  Serial.print(", ");
  Serial.println(colorVectors[index_minimum_value][3]);


  // change servo based on marbleColor
  if (marbleColor == "red" || marbleColor == "yellow" || marbleColor == "white") {
    myservo.write(COL_ONE_ANG);
    delay(10);
  } else if (marbleColor == "blue" || marbleColor == "green" || marbleColor == "black") {
    myservo.write(COL_TWO_ANG);
    delay(10);
  } else {
    myservo.write(COL_OUT_ANG);
    delay(10);
  }
//&& currentMillis - previousMillis >= 2000
  if (marbleColor != "nothing"){
    previousMillis = currentMillis;
    digitalWrite(DROPPER_SOLENOID_PIN, HIGH);  // RELAY ON (extend)
    delay(200);
    digitalWrite(DROPPER_SOLENOID_PIN, LOW);  // RELAY OFF (contract)
    delay(200);
  }                               // wait for marble to drop
}


// pseudocode for building matrix with marbles
//matrix of marble colors to create the image is given. 2d array. each column of marbles is a column vector
// access first index of each column vector. this is the next marble to add to each column
// loop through the first index of each column vector to see if it matches with the marble color.
// if matches with marble color, put marble in that column (move servo to the angle) and remove first index from that column
// if doesn't match any columns, put in trash

