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
const int SERVOFREQ =  60;  // This gives a signal every 20ms
const int SERVOMIN = 150;//205 // this is the 'minimum' pulse length count (out of 4096) approx 1ms
const int SERVOMAX = 600;//410 // this is the 'maximum' pulse length count (out of 4096) approx 2ms
const int nServos = 8;
const int fAngle[8] = {0,0,0,0,0,0,0,0};
const int bAngle[8] = {180,180,180,180,180,180,180,180};

//Input Channels
const int POT1 = 2;
const int POT2 = 3;
const int SWITCH = 6;

//Input Settings
unsigned long lowMicroseconds = 1200;
unsigned long highMicroseconds = 1800;

//Variables for which is the current selected store
int row = 0;
int col = 0;
int switchPos = 1;

unsigned long pulselength;

int angle[nServos];

void setServoAngle(int servoPin,float angle){
  pwm.setPWM(servoPin,0,round(map(angle,0,180,205,410)));
}

int parseInput(int pin,int lowThresh,int highThresh){
  //This will parse out the inputs
  pulselength = pulseIn(pin,HIGH);
  //Serial.println(pulselength);
  //Serial.println(lowThresh);
  //Serial.println(highThresh);

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

  Serial.begin(57600);

  pwm.begin();
  pinMode(POT1, INPUT);
  pinMode(POT2, INPUT);
  pinMode(SWITCH, INPUT);
  
  pwm.setPWMFreq(SERVOFREQ);  // Analog servos run at ~60 Hz updates
}

void loop() {
  delay(100);
  // Check for which pair pots say it is in
  col = parseInput(POT1,lowMicroseconds,highMicroseconds);
  row = parseInput(POT2,lowMicroseconds,highMicroseconds);
  switchPos = parseInput(SWITCH,lowMicroseconds,highMicroseconds);
  //Serial.println(col);
  //Serial.println(row);
  //Serial.println(switchPos);

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
              angle[0] = fAngle[0];
              Serial.println("A1L");
              break;
            case 1:
              //Nothing
              Serial.println("None");
              break;
            case 2:
              //A1R
              Serial.println("A1R");
              angle[1] = fAngle[1];
              break;
          }
          break;
        case 1:
          switch(switchPos){
            case 0:
              //A2L
              Serial.println("A2L");
              angle[2] = fAngle[2];
              break;
            case 1:
              //Nothing
              Serial.println("None");
              break;
            case 2:
              //A2R
              Serial.println("A2R");
              angle[3] = fAngle[3];
              break;
          }
          break;
        case 2: 
          switch(switchPos){
            case 0:
              //A3L
              Serial.println("A3L");
              angle[4] = fAngle[4];
              break;
            case 1:
              //Nothing
              Serial.println("None");
              break;
            case 2:
              //A3R
              Serial.println("A3R");
              angle[5] = fAngle[5];
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
              Serial.println("B1L");
              angle[0] = bAngle[0];
              break;
            case 1:
              //Nothing
              Serial.println("None");
              break;
            case 2:
              //B1R
              Serial.println("B1R");
              angle[1] = bAngle[1];
              break;
          }
          break;
        case 1:
          switch(switchPos){
            case 0:
              //B2L
              Serial.println("B2L");
              angle[6] = fAngle[6];
              break;
            case 1:
              //Nothing
              Serial.println("None");
              break;
            case 2:
              //B2R
              Serial.println("B2R");
              angle[7] = fAngle[7];
              break;
          }
          break;
        case 2: 
          switch(switchPos){
            case 0:
              //B3L
              Serial.println("B3L");
              angle[4] = bAngle[4];
              break;
            case 1:
              //Nothing
              Serial.println("None");
              break;
            case 2:
              //B3R
              Serial.println("B3R");
              angle[5] = bAngle[5];
              break;
          }
          break;
      }
      break;
    case 2: 
      switch(col){
        case 0:
          Serial.println("None");
          break;
        case 1:
          switch(switchPos){
            case 0:
              //C2L
              Serial.println("C2L");
              angle[6] = bAngle[6];
              break;
            case 1:
              //Nothing
              Serial.println("None");
              break;
            case 2:
              //C2R
              Serial.println("C2R");
              angle[7] = bAngle[7];
              break;
          }
          break;
        case 2: 
          Serial.println("None");
          
          break;
      }
      break;
  }

  //set all of the servos
  for(int i = 0; i < nServos; i++){
    setServoAngle(i,angle[i]);
  }
  
}
