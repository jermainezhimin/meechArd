#include <Herkulex.h>

/* Cyclic Pump System
*
* Sketch by Jermaine Cheng
* v. 1.1 - Modularized Cyclic Pump
*
*/  

//////////////
//DEFINITIONS
//////////////

//Pump System
int pState = 1; //0 is initialize, 1 is servo 1 is emptying and servo 2 is drawing

//Servo motor one
//Connect the power to Vin/GND and the TX to RX ad vice versa
int M1 = 200; //motor 1 ID (Default is 253)
int M2 = 253;
int M3 = 100;

//////////////
//   SETUP
//////////////
void setup()  
{
  //Calibrating communications
  Serial.begin(115200);    // Open serial communications
  Herkulex.beginSerial2(115200); //open serial port 1 for communications to both motors
  
  //Rebooting all motors
  Herkulex.reboot(M1); //reboot first motor
  Herkulex.reboot(M2); //reboot first motor
  Herkulex.reboot(M3); //reboot first motor
  Herkulex.torqueON(M1); //allow motor to grind through heavy loads
  Herkulex.torqueON(M2); //allow motor to grind through heavy loads
  Herkulex.torqueON(M3);
  delay(500); 

  //Begining Operations
  Herkulex.initialize(); //initialize motors
}


//////////////
//   LOOP
//////////////
void loop(){
  //Printing Motor Status
  Serial.print(Herkulex.stat(M1));

  //Clear all errors
  if (Herkulex.stat(M1) != 0){
    Herkulex.clearError(M1);
  }
  
  if (Herkulex.stat(M2) != 0){
    Herkulex.clearError(M2);
  }
  
  //Conditional Start
  if (pState == 1){
    Herkulex.moveSpeedOne(M1, 300, 1000, LED_GREEN);
    Herkulex.moveSpeedOne(M2, -300, 1000, LED_RED);
    delay(1000);
    pState = 2;
  } else if (pState == 2) {  
    Herkulex.moveSpeedOne(M1,-300, 1000, LED_RED);
    Herkulex.moveSpeedOne(M2, 300, 1000, LED_GREEN);
    pState = 1;
    delay(1000);
  }
}

