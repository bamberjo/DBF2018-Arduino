/*************************************************** 
DBF 2018 Onboard Arduino Control
Date: 2/12/2019

Description:
This program will use the input from a single channel on the main reciever to release all of the stores from the aircraft in a specified fashion.

//Stores 0-7 require that the servo go forward(either left or right not sure right now)
//Stores n + 8 are the pair on the other side of the servo
 ****************************************************/

//Input Channels
const int signalChannel = 5;//D2
const int signalThresh = 1400;//Microseconds threshold for a positive reading on the signal

const int nServos = 8;
const int nStores = 16;
Servo releaseServos[nServos];
const int releaseOrder = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//Order of release for all of the servos
const int servoChannels = {6,7,8,9,10,11,12,13};//Which channel all of the servos will be connected to 

//Angles for all of the servos
const int leftangle = 20;
const int centerangle = 90;
const int rightangle = 160;

int servoNum = 0;//Counter to keep track of which servo is currently being addressed
bool activatedLast = false;//Makes sure that it does not activate every time through the loop
bool signaled = false;

bool parseInput(int pin,int thresh){
  //This will parse out the inputs
  pulselength = pulseIn(pin,HIGH);

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
    releaseServos[i].write(centerAngle);
  }
  
}

void loop()
{
  delay(10);//Hopefully this will elimate double readings if there is a little bit of noise.
  signaled = parseInput(signalChannel,signalThresh);
  if(signaled && !activatedLast){
    activatedLast = true;//This will make sure that it does not just keep iterating through the stores when a signal is coming in.
    //The store should drop
    //Check if the right or left should be used
    if(releaseOrder[servoNum] > 7){
      //We are in the 'right' stores
      releaseServos[(releaseOrder[servoNum]-8)].write(rightangle);
    }else{
      //We are in the 'left' stores
      releaseServos[releaseOrder[servoNum]].write(leftangle);
    }
    
  }else if(!signaled){
    //Reset the activatedLast because it was not activated in this iteration through the loop.
    
    if(activatedLast){
      //Here the servoNum should be iterated after resetting that servo.
        if(releaseOrder[servoNum] > 7){
          //We are in the 'right' stores
          releaseServos[(releaseOrder[servoNum]-8)].write(centerangle);
        }else{
          //We are in the 'left' stores
          releaseServos[releaseOrder[servoNum]].write(centerangle);
        }

      servoNum++

      //Check if it is time to wrap around
      if(servoNum >= nStores){
        //reset the servoNum
        servoNum = 0;
      }//end if
    }
    
    activatedLast = false;
  }
}
