// color sorting script
// initialize variables
DROPPERSOLENOID = 12;
BLACKCOLUMN; // fill in with servo angle for black column
WHITECOLUMN; // fill in with servo angle for white column
BIN; // fill in with servo angle for marble disposal

void setup(){   
  pinMode(DROPPERSOLENOID,OUTPUT); // RELAY PIN   
  digitalWrite(DROPPERSOLENOID,HIGH); // Normally ON Only For Chanies Relay Module   
}  

void loop(){
  
  // color sensors determines color of marble. create "marbleColour"
  // determine where marble should go
  if (marbleColor == black){
    //servo goes to BLACKCOLUMN
  }
  else if (marbleColor == white){
    // servo goes to WHITECOLUMN
  }
  else{
    // servo tosses marble out
  }

  // solenoid pushes marble out
  pulseSolenoid(DROPPERSOLENOID);


}

// solenoid pushes marble out
void pulseSolenoid(int RELAYPIN){
  digitalWrite(RELAYPIN,LOW); // RELAY ON   
  delay(200);   
  digitalWrite(RELAYPIN,HIGH); // RELAY OFF   
  delay(50);
}


