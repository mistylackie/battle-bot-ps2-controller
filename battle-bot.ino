// PS2X_lib - Version: Latest 
#include <PS2X_lib.h>

PS2X ps2x; // create PS2 Controller Class

//The library does NOT support hot pluggable controllers.
//You must ALWA either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;
int rightSpeed = 255;
int leftSpeed = 255;
int turnSpeed = 150;

#define IN1  7   //K1、K2 motor direction
#define IN2  8   //K1、K2 motor direction
#define IN3  9   //K3、K4 motor direction
#define IN4  4   //K3、K4 motor direction
#define ENA  5   // Needs to be a PWM pin to be able to control motor speed ENA
#define ENB  6    // Needs to be a PWM pin to be able to control motor speed ENA
#define FLIPPER 3 

void setup(){
  Serial.begin(9600);

  /******L298N******/
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);
  pinMode(FLIPPER, OUTPUT);

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 
  if(error == 0){
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }
   
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
  //Serial.print(ps2x.Analog(1), HEX);
   
  type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }
  
}

void loop(){
   /* You must Read Gamepad to get new values
   Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values
   
   you should call this at least once a second
   */
   
   
   
 if(error == 1) //skip loop if no controller found
  return;

 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
        
    if(ps2x.Button(PSB_PAD_UP)) { //will be TRUE as long as button is pressed
      analogWrite(ENA,leftSpeed); //lspeed:0-255
      analogWrite(ENB,rightSpeed); //rspeed:0-255
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4,LOW);
      Serial.print("up");
    } else if(ps2x.Button(PSB_PAD_RIGHT)) {
      analogWrite(ENA,turnSpeed); //lspeed:0-255
      analogWrite(ENB,turnSpeed); //rspeed:0-255
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.print("right");
    } else if(ps2x.Button(PSB_PAD_LEFT)) {
      analogWrite(ENA,turnSpeed); //lspeed:0-255
      analogWrite(ENB,100); //rspeed:0-255
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.print("left");
    } else if(ps2x.Button(PSB_PAD_DOWN)) {
      analogWrite(ENA,leftSpeed); //lspeed:0-255
      analogWrite(ENB,rightSpeed); //rspeed:0-255
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.print("down");
    } else if(ps2x.Button(PSB_BLUE)) {
      digitalWrite(FLIPPER, LOW);
      Serial.print("FLIPPER");
    } else {
      analogWrite(ENA,0); //lspeed:0-255
      analogWrite(ENB,0); //rspeed:0-255
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      digitalWrite(FLIPPER, HIGH);
      Serial.print("none");
    }
 }
 delay(50);
}
