/*************************************************** 
DBF 2018 Onboard Arduino Control
Date: 2/12/2019

Description:
This program will use the input from a single channel on the main reciever to release all of the stores from the aircraft in a specified fashion.

//Stores 0-7 require that the servo go forward(either left or right not sure right now)
//Stores n + 8 are the pair on the other side of the servo
 ****************************************************/

#include "Servo.h"

//Input Channels
const int signalChannel = 8;//D2
const int signalThresh = 1800;//Microseconds threshold for a positive reading on the signal

const int nServos = 6;
const int nStores = nServos*2;
Servo releaseServos[nServos];
const int releaseOrder[nStores] = {0,1,2,3,4,5,6,7,8,9,10,11};//Order of release for all of the servos
const int servoChannels[nServos] = {2,3,4,5,6,7};//Which channel all of the servos will be connected to 

//Angles for all of the servos
const int secondangles[nServos] = {30,30,30,30,150,30};
const int centerangles[nServos] = {100,100,100,100,100,100};
const int firstangles[nServos] = {50,50,50,50,130,50};

int storeNum = 0;//Counter to keep track of the current store
int servoNum = 0;//Counter to keep track of which servo is currently being addressed
bool activatedLast = false;//Makes sure that it does not activate every time through the loop
bool signaled = false;

bool parseInput(int pin,int thresh){
  //This will parse out the inputs
  int pulselength = pulseIn(pin,HIGH);

  if(pulselength < thresh){ 
    return false;
  }else{
    return true;
  }
}

void setup()
{
  //Initialize all of the servos
  for(int i = 0; i < nServos; i++){
    releaseServos[i].attach(servoChannels[i]);
    releaseServos[i].write(centerangles[servoNum]);
  }

  Serial.begin(9600);
  
}

void loop()
{
  //Serial.println("Working");
  delay(10);//Hopefully this will elimate double readings if there is a little bit of noise.
  signaled = parseInput(signalChannel,signalThresh);
  if(signaled && !activatedLast){
    Serial.println("Part1");
    activatedLast = true;//This will make sure that it does not just keep iterating through the stores when a signal is coming in.
    //The store should drop
    //Check if the right or left should be used
    if(releaseOrder[storeNum] >= nServos){
      servoNum = storeNum - (nServos-1);
      //We are in the 'second' stores
      releaseServos[releaseOrder[servoNum]].write(secondangles[servoNum]);
      Serial.print((releaseOrder[servoNum]));
      Serial.print("angle:");
      Serial.println(secondangles[servoNum]);
    }else{
      //We are in the 'first' stores
      servoNum = storeNum;
      releaseServos[releaseOrder[servoNum]].write(firstangles[servoNum]);
      Serial.print((releaseOrder[servoNum]));
      Serial.print("angle:");
      Serial.println(firstangles[servoNum]);
    }
    
  }else if(!signaled){
    //Reset the activatedLast because it was not activated in this iteration through the loop.
    
    if(activatedLast){
      //Here the servoNum should be iterated after resetting that servo.
      releaseServos[(releaseOrder[servoNum])].write(centerangles[servoNum]);
      Serial.print((releaseOrder[servoNum]));
      Serial.println("centered");
        /*if(releaseOrder[servoNum] > 5){
          servoNum = storeNum - (nServos-1);
          //We are in the 'second' stores
          releaseServos[(releaseOrder[servoNum])].write(centerangles[servoNum]);
          Serial.print((releaseOrder[servoNum]));
          Serial.println("centered");
        }else{
          //We are in the 'first' stores
          servoNum = storeNum;
          releaseServos[releaseOrder[servoNum]].write(centerangles[servoNum]);
          
        }*/

      storeNum++;

      //Check if it is time to wrap around
      if(storeNum >= nStores){
        //reset the servoNum
        storeNum = 0;
      }//end if
    }
    
    activatedLast = false;
  }
}
