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

//Communication
String inData;
char sys;
char mode;
int reFill = 0;

//Pump System
int pState = 1; //1 is initialize, 2 is servo 1 is emptying and servo 2 is drawing
int pSpeed = 500;
int pDiff = 250;

//Servo motor one
//Connect the power to Vin/GND and the TX to RX ad vice versa
int M1 = 253; //motor 1 ID (Default is 253)
int eStop1 = 23;
int sStop1 = 22;

//Servo motor two
//Connect the jumper to form serial bus to motor 1
int M2=100; //motor 2 ID (Default is 253)
int eStop2 = 25;
int sStop2 = 24;

//Coagulant system
int cState = 1; // 1 is initialized and 
int cSpeed = 200;

//Servo motor three
//Connect the jumper to form serial bus to motor 1
int M3=200; //motor 2 ID (Default is 253)
int cStart = 26;
int cEnd = 27;

//Heart Meter
//  Variables
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

//////////////
//   SETUP
//////////////
void setup()  
{  
  //Calibrating communications
  Serial.begin(115200);    // Open serial communications
  interruptSetup();   
  Herkulex.beginSerial2(115200); //open serial port 2 for communications to both motors
  
  //Rebooting all motors
  Herkulex.reboot(M1); //reboot first motor 
  Herkulex.reboot(M2); //reboot second motor
  Herkulex.reboot(M2); //reboot second motor
  
  Herkulex.torqueON(M1); //allow motor to grind through heavy loads
  Herkulex.torqueON(M2); //allow motor to grind through heavy loads
  Herkulex.torqueON(M2); //allow motor to grind through heavy loads
  
  delay(500);

  //Begining Operations
  Herkulex.initialize(); //initialize motors
}


//////////////
//   LOOP
//////////////
void loop(){
  comms_main();
  error_main();
  pump_main();
  coagulant_main();
}


//////////////
// ADD MODULES
//////////////

//Delay (100)
void comms_main(){
  while (Serial.available() > 0){
    char recieved = Serial.read();
    inData += recieved; 
    
    // Process message when new line character is recieved
    if (recieved == '\n'){
      
      //Start Communication Parsing
      parse_main();
      
      inData = ""; // Clear recieved buffer
 s   }
  } delay(100);
}

//Delay (0)
void parse_main(){
  char sys = inData[0];
  char mode = inData[1];

  //Parsing the blood system
  if (sys == 'B'){
    Serial.print(sys);
    Serial.println(mode);
    if (mode == 'I'){
      pState = 3;
    } else if (mode == '0'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 0;
      pDiff = 0;
    } else if (mode == '1'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 100;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '2'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 200;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '3'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 300;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '4'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 400;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '5'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 500;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '6'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 600;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '7'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 700;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '8'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 800;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    } else if (mode == '9'){
      if (pState == 3){
        pState = 1; 
      }
      pSpeed = 900;
      pDiff = 0.5 * pSpeed;
      cSpeed = 0.15 *pSpeed;
    }
    
   //Parsing for the anticoagulant 
  } else if (sys == 'C') {
    Serial.print(sys);
    Serial.println(mode);
    if (mode == 'I'){
      cState = 2;
    } else if (mode == '0'){
      cState = 1;
      cSpeed = 0;
    } else if (mode == '1'){
      cState = 1;
      cSpeed = 0.15 *pSpeed;
    }
  } else if (sys == 'Q') { 
    if (mode == '1'){
      Serial.println(pSpeed);
    } else if (mode == '0'){
      Serial.println(BPM);
    } else if (mode == '2'){
      if (reFill == 1) {
        Serial.println("Refill");
      } else {
        Serial.println("None");
      }
    }
  } else {
    Serial.println("WT");
  }
  sys = ' ';
  mode = ' ';
}

//Delay (0)
void error_main(){
  if ((Herkulex.stat(M1) != 0) || (Herkulex.stat(M2) != 0) || (Herkulex.stat(M3) != 0) ) {
    //Serial.println("Glitch Detected");
    Herkulex.clearError(M1);
    Herkulex.clearError(M2);
    Herkulex.clearError(M3);
  }
}

//Delay (200)
void pump_main(){
  
  //////////////////////////////////////////////////  
  //State 1 is servo 1 => emptying | servo 2 => drawing
  //////////////////////////////////////////////////
  if ((pState == 1)) {
    
    if ((digitalRead(eStop1) == LOW) && (digitalRead(sStop2) == LOW)){
      Herkulex.moveSpeedOne(M1, -pSpeed, 200, LED_BLUE);
      Herkulex.moveSpeedOne(M2, pSpeed + pDiff , 200, LED_GREEN);
      delay(200);
      
    } else if ((digitalRead(eStop1) == HIGH) && (digitalRead(sStop2) == LOW)){
      Herkulex.moveSpeedOne(M1, 0, 200, LED_RED);
      Herkulex.moveSpeedOne(M2, pSpeed + pDiff, 200, LED_GREEN); 
      delay(200);     
      
    } else if ((digitalRead(eStop1) == LOW) && (digitalRead(sStop2) == HIGH)){
      Herkulex.moveSpeedOne(M1, -pSpeed, 200, LED_BLUE);
      Herkulex.moveSpeedOne(M2, 0, 200, LED_RED);
      delay(200);
        
    } else if ((digitalRead(eStop1) == HIGH) && (digitalRead(sStop2) == HIGH)){
      Herkulex.moveSpeedOne(M1, 0, 200, LED_RED);
      Herkulex.moveSpeedOne(M2, 0, 200, LED_RED); 
      delay(200);
      pState = 2;
    }
    
  //////////////////////////////////////////////////  
  //State 2 is servo 2 => emptying | servo 1 => drawing
  //////////////////////////////////////////////////
  } else if ((pState == 2)) {
    
    if ((digitalRead(eStop2) == LOW) && (digitalRead(sStop1) == LOW)){
      Herkulex.moveSpeedOne(M1,pSpeed + pDiff, 200, LED_GREEN);
      Herkulex.moveSpeedOne(M2,-pSpeed, 200, LED_BLUE);
      delay(200);
      
    } else if ((digitalRead(eStop2) == HIGH) && (digitalRead(sStop1) == LOW)){
      Herkulex.moveSpeedOne(M1,pSpeed + pDiff, 200, LED_GREEN); 
      Herkulex.moveSpeedOne(M2,0, 200, LED_RED);
      delay(200);     
      
    } else if ((digitalRead(eStop2) == LOW) && (digitalRead(sStop1) == HIGH)){
      Herkulex.moveSpeedOne(M1,0, 200, LED_RED);
      Herkulex.moveSpeedOne(M2,-pSpeed, 200, LED_BLUE);
      delay(200);
        
    } else if ((digitalRead(eStop2) == HIGH) && (digitalRead(sStop1) == HIGH)){
      Herkulex.moveSpeedOne(M1, 0, 200, LED_RED);
      Herkulex.moveSpeedOne(M2, 0, 200, LED_RED); 
      delay(200);
      pState = 1;
    }

  //////////////////////////////////////////////////  
  // Initializing State
  //////////////////////////////////////////////////
  } else if ((pState == 3)) {
    if ((digitalRead(eStop1) == LOW) && (digitalRead(sStop2) == LOW)){
      Herkulex.moveSpeedOne(M1, -500, 200, LED_BLUE);
      Herkulex.moveSpeedOne(M2, 750 , 200, LED_GREEN);
      delay(200);
      
    } else if ((digitalRead(eStop1) == HIGH) && (digitalRead(sStop2) == LOW)){
      Herkulex.moveSpeedOne(M1, 0, 200, LED_RED);
      Herkulex.moveSpeedOne(M2, 750, 200, LED_GREEN); 
      delay(200);     
      
    } else if ((digitalRead(eStop1) == LOW) && (digitalRead(sStop2) == HIGH)){
      Herkulex.moveSpeedOne(M1, -500, 200, LED_BLUE);
      Herkulex.moveSpeedOne(M2, 0, 200, LED_RED);
      delay(200);
        
    } else if ((digitalRead(eStop1) == HIGH) && (digitalRead(sStop2) == HIGH)){
      Herkulex.moveSpeedOne(M1, 0, 200, LED_RED);
      Herkulex.moveSpeedOne(M2, 0, 200, LED_RED); 
      delay(200);
    }
  }
}

//Delay (200)
void coagulant_main(){
  if (cSpeed < 100){
    cSpeed = 100;
  }
  if (cState == 1){
    if (digitalRead(cStart) == LOW){
      Herkulex.moveSpeedOne(M3, cSpeed, 200, LED_GREEN);
      delay(200);
    } else if (digitalRead(cStart) == HIGH){
      Herkulex.moveSpeedOne(M3, 0, 200, LED_GREEN);
      reFill = 1;
      delay(200);
      
    }
  } else if (cState == 2){
    if (digitalRead(cEnd) == LOW){
      Herkulex.moveSpeedOne(M3, -800, 200, LED_GREEN);
      delay(200);
    } else if (digitalRead(cEnd) == HIGH){
      Herkulex.moveSpeedOne(M3, 0, 200, LED_GREEN);
      reFill = 0;
      delay(200);
    }
  } 
}

