// color sorting with euclidean distance for sprint 2 mechanical system (3 servos, 8x8 matrix)
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
String marbleColor;
float cMultiplier = 0.02;
Servo topservo;  // create Servo object to control a servo
Servo rightservo;
Servo leftservo;
int pos = 0;    // variable to store the servo position

// initialize variables
int DROPPER_SOLENOID_PIN = 12;
int TOP_SERVO_PIN = 3;
int RIGHT_SERVO_PIN = 5;
int LEFT_SERVO_PIN = 6;
int COLUMNS [9][2]= {{35, 35}, {77, 35}, {77, 70}, {77, 100}, {77, 135}, {123, 30}, {123, 63}, {123, 95}, {123, 130}}; //first chute is trash, rest go left to right

//matrix of marble colors to create the image is given. 2d array. each column of marbles is a column vector
char *image [8][8] = {{"red", "red", "red", "red", "red", "red", "red", "red"}, {"yellow", "yellow", "yellow", "yellow", "yellow", "yellow", "yellow", "yellow"}, {"green", "green", "green", "green", "green", "green", "green", "green"}, {"blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue"}, {"white","white", "white", "white", "white", "white", "white", "white"}, {"black", "black", "black", "black", "black", "black", "black", "black"}, {"red", "yellow", "black", "red", "yellow", "black", "red", "yellow"}, {"green", "blue", "white", "green", "blue", "white", "green", "blue"}};
int fillCount[] = {0, 0, 0, 0, 0, 0, 0, 0}; // index of image column to start at for each column


float colorVectors[7][4] = {
  //rgbc values for the different marble colors
  {2207, 2083, 1389, 5474},  // white
  {165, 148, 101, 396},      // black
  {1275, 395, 313, 1808},     // red
  {597, 770, 343, 1674},    // green
  {418, 565, 590, 1495},     // blue
  {2513, 1854, 719, 4962},   // yellow
  {76, 69, 51, 189}          // nothing
};

char *colorLabels[] = { "white", "black", "red", "green", "blue", "yellow", "nothing" };


void setup() {
  // put your setup code here, to run once:


  Serial.begin(9600);
  pinMode(DROPPER_SOLENOID_PIN, OUTPUT);     // RELAY PIN
  digitalWrite(DROPPER_SOLENOID_PIN, LOW);  // Normally ON Only For Chanies Relay Module
  topservo.attach(TOP_SERVO_PIN);         // attaches the servo on pin 3 to the Servo object
  leftservo.attach(LEFT_SERVO_PIN);
  rightservo.attach(RIGHT_SERVO_PIN);

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

void servoColumn(int column){
  // moves the servos so that the marble falls into the desired column
     if (column >= 5){
      topservo.write(COLUMNS[column][0]);
      rightservo.write(COLUMNS[column][1]);
    } else if (1 <= column <= 4){
      topservo.write(COLUMNS[column][0]);
      leftservo.write(COLUMNS[column][1]);
    } else {
      topservo.write(COLUMNS[0][0]);
    }
}


void loop() {
  delay(1000);
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
  /*
  Serial.print(marbleColor);
  Serial.print(": ");
  Serial.print(colorVectors[index_minimum_value][0]);
  Serial.print(", ");
  Serial.print(colorVectors[index_minimum_value][1]);
  Serial.print(", ");
  Serial.print(colorVectors[index_minimum_value][2]);
  Serial.print(", ");
  Serial.println(colorVectors[index_minimum_value][3]);
  */



// putting marbles in the right spot
  if (marbleColor != "nothing"){
    // access first index of each column vector. this is the next marble to add to each column
    bool match = false;
    for (int i = 0; i <= 7; i++){
      if (marbleColor == image[i][fillCount[i]]){
        servoColumn(i+1);
        fillCount[i]++;
        match = true;
    }
    if (match == false){
      servoColumn(0);
    }
    
    digitalWrite(DROPPER_SOLENOID_PIN, HIGH);  // RELAY ON (extend)
    delay(200);
    digitalWrite(DROPPER_SOLENOID_PIN, LOW);  // RELAY OFF (contract)
    delay(200);
  }   
}


// marble sorting code (we know this works)
/*
if (marbleColor == "red"){
      topservo.write(COLUMNS[7][0]);
      rightservo.write(COLUMNS[7][1]);
    } else if (marbleColor == "green"){
      topservo.write(COLUMNS[8][0]);
      rightservo.write(COLUMNS[8][1]);
    } else if (marbleColor == "blue"){
      topservo.write(COLUMNS[3][0]);
      leftservo.write(COLUMNS[3][1]);
    } else if (marbleColor == "yellow"){
      topservo.write(COLUMNS[4][0]);
      leftservo.write(COLUMNS[4][1]);
    } else if (marbleColor == "black"){
      topservo.write(COLUMNS[5][0]);
      rightservo.write(COLUMNS[5][1]);
    } else if (marbleColor == "white"){
      topservo.write(COLUMNS[6][0]);
      rightservo.write(COLUMNS[6][1]);
    }
    else{
      topservo.write(COLUMNS[0][0]);
    }
    */


