#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
String marbleColor;
float cMultiplier = 0.02;
/*
float colorVectors[7][4] = { //red, green, blue, c values for the different marble colors
  { 38, 40, 26, 12141*cMultiplier},  // white 
  { 39, 39, 26, 595*cMultiplier},   // black 
  { 71, 22, 18, 2000*cMultiplier},   // red 
  { 34, 47, 21, 2143*cMultiplier},  // green 
  { 26, 38, 41, 2185*cMultiplier},   // blue 
  { 48, 4, 15, 5945*cMultiplier},   // yellow 
  { 39, 36, 27, 193*cMultiplier }    // nothing 
};*/

float colorVectors[7][4] = { //rgbc values for the different marble colors
  {2299, 2212, 1455, 5720},  // white 
  {175, 152, 107, 415},   // black 
  {995, 337, 253, 1459},   // red 
  {844, 1074, 509, 2361},  // green 
  {433, 608, 673, 1615},   // blue 
  {2321, 1829, 722, 4740},   // yellow 
  {76, 70, 52, 193}    // nothing 
};

char *colorLabels[] = { "white", "black", "red", "green", "blue", "yellow", "nothing" };


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);


  if (tcs.begin()){
    Serial.println("found sensor");
  } else {
    Serial.println("no sensor found");
    while (1); // end program
  }
}


void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  float sum = float(c)*cMultiplier;
  float red = r*100/ float(c);
  float green = g*100/ float(c);
  float blue = b*100/ float(c);
  float marbleVector[4] = {r, g, b, c};

   Serial.print("raw marble values "); Serial.print(r); Serial.print(", "); Serial.print(g); Serial.print(", "); Serial.print(b); Serial.print(", "); Serial.println(c);

  // find euclidean distance of rgb of this marble to the reference for every marble color
  float allDistances[7];
  for (int i = 0; i < 7; i++) {
    allDistances[i] = euclideanDistance(marbleVector, colorVectors[i]);
    Serial.print(allDistances[i]); Serial.print(", ");
  }
  Serial.println(" ");


  // find closest distance (scale down c basis vector because its less important)
  int index_minimum_value = minValue(allDistances);

  // find marble color from closest distance
  marbleColor = colorLabels[index_minimum_value];
  Serial.print(marbleColor); Serial.print(": ");
  Serial.print(colorVectors[index_minimum_value][0]); Serial.print(", ");
  Serial.print(colorVectors[index_minimum_value][1]); Serial.print(", ");
  Serial.print(colorVectors[index_minimum_value][2]); Serial.print(", ");
  Serial.println(colorVectors[index_minimum_value][3]);
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
  /*
  Serial.print("The minimum value of the array is: ");
  Serial.print(minVal);
  Serial.print(" at index: ");
  Serial.println(minValIndex);*/
  return minValIndex;
  
}

float euclideanDistance(float *marbleVec, float *refVec) {
  // give two 4d vectors and it spits out the euclidean distance of them
  return sqrt(pow(marbleVec[0]-refVec[0], 2) + pow(marbleVec[1]-refVec[1], 2) + pow(marbleVec[2]-refVec[2], 2) + pow((marbleVec[3]-refVec[3])*cMultiplier, 2));
}
