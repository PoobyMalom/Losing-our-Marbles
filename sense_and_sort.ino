// color sorting with euclidean distance for sprint 2 mechanical system (3 servos, 8x8 matrix)
// change image by updating the 2D array "image" 
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
String marbleColor;
float cMultiplier = 0.02;
Servo topservo;  // create Servo object to control a servo
Servo rightservo;
Servo leftservo;

// initialize variables
int SCREW_MOTOR_PIN = 11;
int DROPPER_SOLENOID_PIN = 12;
int TOP_SERVO_PIN = 3;
int RIGHT_SERVO_PIN = 5;
int LEFT_SERVO_PIN = 6;
int COLUMNS [9][2]= {{0, 12}, {48, 17}, {48, 47}, {48, 82}, {48, 118}, {103, 58}, {103, 97}, {103, 133}, {103, 168}}; //first chute is trash, rest go left to right



// example images
// lava creeper face
//char *image [8][8] = {{"yellow","yellow","yellow","red","red","red","red","yellow",},{"yellow","yellow","red","red","black','black','yellow','yellow',},{'black','black','black','red','black','black','red','red',},{'red','black','black','black','red','red','red','red',},{'red','black','black','black','yellow','red','red','red',},{'black','black','black','yellow','black','black','red','red',},{'red','red','yellow','yellow','black','black','red','red',},{'red','red','red','red','red','red','red','red'}};
// flowers
//char *image [8][8] = {{"green","blue","blue","blue","red","blue","white","white"},{"green","green","green","red","yellow","red","white","white"},{"green","blue","blue","blue","red","blue","white","blue"},{"green","blue","blue","blue","white","blue","blue","blue"},{"green","blue","blue","red","white","white","blue","blue"},{"green","blue","red","yellow","red","white","blue","blue"},{"green","green","green","red","white","blue","yellow","yellow"},{"green","blue","blue","blue","blue","blue","yellow","yellow"}};
// car
//char *image [8][8] = {{"blue","blue","green","red","blue","blue","blue","white"},{"blue","black","green","green","green","blue","white","white"},{"black","red","black","green","green","blue","blue","white"},{"blue","black","green","green","green","blue","blue","blue"},{"blue","blue","green","green","green","blue","blue","white"},{"blue","black","green","green","green","blue","white","white"},{"black","red","black","green","blue","white","yellow","yellow"},{"blue","black","green","yellow","blue","white","yellow","yellow"}};
// smiley
//char *image [8][8] = {{"blue","blue","blue","blue","black","black","black","green"},{"green","red","red","white","white","white","black","green"},{"green","red","white","white","blue","white","black","green"},{"green","red","white","yellow","white","white","black","green"},{"green","red","white","yellow","white","white","black","green"},{"green","red","white","white","blue","white","black","green"},{"green","red","red","white","white","white","black","green"},{"blue","blue","blue","blue","black","black","black","green"}};
// symmetrical pattern
// char *image [8][8] = {{"yellow","white","blue","black","green","red","blue","black"},{"white","yellow","black","green","red","blue","black","blue"},{"black","white","yellow","red","blue","black","blue","red"},{"blue","black","white","yellow","black","blue","red","green"},{"red","blue","black","white","yellow","red","green","black"},{"green","red","blue","black","white","yellow","black","blue"},{"yellow","green","red","blue","black","white","yellow","white"},{"white","yellow","green","red","blue","black","white","yellow"}};
// one color per column test
//char *image [8][8] = {{"red", "red", "red", "red", "red", "red", "red", "red"}, {"yellow", "yellow", "yellow", "yellow", "yellow", "yellow", "yellow", "yellow"}, {"green", "green", "green", "green", "green", "green", "green", "green"}, {"blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue"}, {"white","white", "white", "white", "white", "white", "white", "white"}, {"black", "black", "black", "black", "black", "black", "black", "black"}, {"red", "yellow", "black", "red", "yellow", "black", "red", "yellow"}, {"green", "blue", "white", "green", "blue", "white", "green", "blue"}};
// snake
//char *image [8][8] = {{"blue", "blue", "green", "green", "green", "green", "green", "green"}, {"blue", "green", "green", "green", "green", "green", "green", "green"}, {"blue", "green", "green", "green", "green", "green", "blue", "blue"}, {"blue", "green", "green", "green", "green", "green", "blue", "blue"}, {"blue", "blue", "blue", "green", "green", "green", "blue", "blue"}, {"black", "green", "black", "green", "green", "green", "blue", "blue"}, {"green", "green", "green", "blue", "blue", "blue", "blue", "blue"},{"red", "green", "blue", "blue", "blue", "blue", "blue", "blue"}};
// mushroom
//char *image [8][8] =  {{"blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue"}, {"blue", "blue", "red", "yellow", "white", "green", "blue", "blue"}, {"blue", "black", "red", "red", "red", "yellow", "green", "blue"}, {"blue", "yellow", "red", "red", "white", "white", "white", "blue"},{"blue", "yellow", "red", "red", "white", "white", "white", "blue"}, {"blue", "black", "red", "red", "red", "yellow", "green", "blue"},{"blue", "blue", "red", "yellow", "white", "green", "blue", "blue"},{"blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue"}};

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
//char *colorLabels[] = { 'white', 'black', 'red', 'green', 'blue', 'yellow', 'nothing' };

void setup() {
  // put your setup code here, to run once:


  Serial.begin(9600);
  pinMode(SCREW_MOTOR_PIN, OUTPUT);     // DC MOTOR PIN
  pinMode(DROPPER_SOLENOID_PIN, OUTPUT);     // RELAY PIN
  digitalWrite(SCREW_MOTOR_PIN, HIGH);  // RELAY OFF (motor off)
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
    
  }


  // find closest distance (scale down c basis vector because its less important)
  int index_minimum_value = minValue(allDistances);

  // find marble color from closest distance
  marbleColor = colorLabels[index_minimum_value];

  Serial.println(marbleColor);




// putting marbles in the right spot
  if (marbleColor != "nothing"){
    // access first index of each column vector. this is the next marble to add to each column
    bool match = false;
    for (int i = 0; i <= 7; i++){
      if ((marbleColor == image[i][fillCount[i]]) && (match == false) && fillCount[i] < 8){
        servoColumn(i+1);
        fillCount[i]++;
        match = true;
        Serial.print("column: "); Serial.println(i);
    }
  }
  if (match == false){
    servoColumn(0);
    Serial.print("column: "); Serial.println("trash");
  }
    
    digitalWrite(DROPPER_SOLENOID_PIN, HIGH);  // RELAY ON (extend)
    delay(200);
    digitalWrite(DROPPER_SOLENOID_PIN, LOW);  // RELAY OFF (contract)

    
  } else{
    digitalWrite(SCREW_MOTOR_PIN, LOW);  // RELAY ON (motor on)
    delay(2000);
    digitalWrite(SCREW_MOTOR_PIN, HIGH);  // RELAY OFF (motor off)
  }

}
