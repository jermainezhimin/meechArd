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
int M1 = 253; //motor 1 ID (Default is 253)

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
  Herkulex.torqueON(M1); //allow motor to grind through heavy loads
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
  
  //Conditional Start
  if (pState == 1){
    Herkulex.moveSpeedOne(M1, 1000, 500, LED_GREEN);
    delay(5000);
    pState = 2;
  } else if (pState == 2) {  
    //Herkulex.moveSpeedOne(M1, 1000, 500, LED_RED);
    pState = 1;
    delay(5000);
  }
}

