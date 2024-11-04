// color sorting script
// future improvements: add safety to ensure solenoid isn't on for more than 1s
#include <Servo.h>


// initialize variables
int DROPPER_SOLENOID_PIN = 12;
int DROPPER_SERVO_PIN = 5;
int BLACK_COL_ANG = 45; // servo angle for black column (no gear ratios)
int WHITE_COL_ANG = 80; // servo angle for white column (no gear ratios)
int OUT_COL_ANG = 0; // servo angle for marble disposal
int SOLENOID_ON_TIME = 100;

Servo myservo;  // create Servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup(){   
  pinMode(DROPPER_SOLENOID_PIN,OUTPUT); // RELAY PIN   
  digitalWrite(DROPPER_SOLENOID_PIN,HIGH); // Normally ON Only For Chanies Relay Module   
  myservo.attach(DROPPER_SERVO_PIN);  // attaches the servo on pin 5 to the Servo object
}  

void loop(){
  digitalWrite(DROPPER_SOLENOID_PIN,LOW); // RELAY ON. DONT POWER FOR MORE THAN 1 SEC
  // color sensors determines color of marble. create "marbleColour"
  //if (marbleColor == black){
    myservo.write(BLACK_COL_ANG);
  //}
  //else if (marbleColor == white){
    myservo.write(WHITE_COL_ANG);
  //}
  //else {
    myservo.write(OUT_COL_ANG);
  //}
  digitalWrite(DROPPER_SOLENOID_PIN,HIGH); // RELAY OFF
  
}


