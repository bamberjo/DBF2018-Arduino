/*************************************************** 
DBF 2018 Onboard Arduino Control
Date: 11/7/2018

Description:
This program will parse three input channels from the reciever into the arduino and will then control the output to the servos via servo driver board

Payload Release Layout

_________ 1 __________ 2 __________ 3 __________
|
|
|
A    A1L-----A1R   A2L-----A2R   A3L-----A3R
|    |        |    |        |    |        |    
|    S0       S1   S2       S3  S4       S5
|    |        |                  |        | 
B    B1L-----B1R   B2L-----B2R   B3L-----B3R
|                  |        |
|                  S6       S7
|                  |        |
C                  C2L----C2R
|
|


Controller Scheme

Pot1(Ch.6)(DIO 3): Columns
0-15: 1
15-85: 2
85-100: 3

Pot2(Ch.7)(DIO 4): Rows
0-15: A
15-85: B
85-100: C

Switch(Ch.8)(DIO 5): Release
Nuetral: Nothing
Forward: Release Left
Backward: Release Right
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//Initialize the PWM ServoDriver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


//Servo Settings
const int SERVOFREQ 50  // This gives a signal every 20ms
const int SERVOMIN  205 // this is the 'minimum' pulse length count (out of 4096) approx 1ms
const int SERVOMAX  410 // this is the 'maximum' pulse length count (out of 4096) approx 2ms
const int nServos = 8;
const int fAngle = 0;
const int bAngle = 180;

//Input Channels
const int POT1 = 3;
const int POT2 = 4;
const int SWITCH = 5;

//Input Settings
const int LOWTHRESH = 15;
const int HIGHTHRESH = 85;

unsigned long lowMicroseconds = 2000*LOWTHRESH/100;
unsigned long highMicroseconds = 2000*HIGHTHRESH/100;

//Variables for which is the current selected store
int row = 0;
int col = 0;
int switchPos = 1;

unsigned long pulselength;

int angle[nServos];

void setServoAngle(servoPin,angle){
  pwm.setPWM(servoPin,0,round(map(angle,0,180,205,410)));
}

int parseInput(pin,lowThresh,highThresh){
  //This will parse out the inputs
  pulselength = pulseIn(pin,HIGH);

  if(pulselength < lowThresh){
    return 0;
  }else if(pulselength > highThresh){
    return 2;
  }else{
    return 1;
  }
}

// our servo # counter
uint8_t servonum = 0;

void setup() {

  pwm.begin();
  
  pwm.setPWMFreq(SERVOFREQ);  // Analog servos run at ~60 Hz updates
}

void loop() {
  // Check for which pair pots say it is in
  col = parseInput(POT1,lowMicroseconds,highMicroseconds);
  row = parseInput(POT2,lowMicroseconds,highMicroseconds);
  switchPos = parseInput(SWITCH,lowMicroseconds,highMicroseconds);

  //Set all angles to their default
  for(int i = 0; i < nServos; i++){
   angle[i] = 90;
  }

  //Set the Servo if necessary
  switch(row){
    case 0:
      switch(col){
        case 0:
          switch(switchPos){
            case 0:
              //A1L
              angle[0] = fAngle;
              break;
            case 1:
              //Nothing
              break;
            case 2;
              //A1R
              angle[1] = fAngle;
              break;
          }
          break;
        case 1:
          switch(switchPos){
            case 0:
              //A2L
              angle[2] = fAngle;
              break;
            case 1:
              //Nothing
              break;
            case 2;
              //A2R
              angle[3] = fAngle;
              break;
          }
          break;
        case 2: 
          switch(switchPos){
            case 0:
              //A3L
              angle[4] = fAngle;
              break;
            case 1:
              //Nothing
              break;
            case 2;
              //A3R
              angle[5] = fAngle;
              break;
          }
          break;
      }
      break;
    case 1:
      switch(col){
        case 0:
          switch(switchPos){
            case 0:
              //B1L
              angle[0] = bAngle;
              break;
            case 1:
              //Nothing
              break;
            case 2;
              //B1R
              angle[1] = bAngle;
              break;
          }
          break;
        case 1:
          switch(switchPos){
            case 0:
              //B2L
              angle[6] = fAngle;
              break;
            case 1:
              //Nothing
              break;
            case 2;
              //B2R
              angle[7] = fAngle;
              break;
          }
          break;
        case 2: 
          switch(switchPos){
            case 0:
              //B3L
              angle[4] = bAngle;
              break;
            case 1:
              //Nothing
              break;
            case 2;
              //B3R
              angle[5] = bAngle;
              break;
          }
          break;
      }
      break;
    case 2: 
      switch(col){
        case 0:
          
          break;
        case 1:
          switch(switchPos){
            case 0:
              //B2L
              angle[6] = bAngle;
              break;
            case 1:
              //Nothing
              break;
            case 2;
              //B2R
              angle[7] = bAngle;
              break;
          }
          break;
        case 2: 
          
          break;
      }
      break;
  }

  //set all of the servos
  for(int i = 0; i < nServos; i++){
    setServoAngle(i,angle[i]);
  }
  
}
