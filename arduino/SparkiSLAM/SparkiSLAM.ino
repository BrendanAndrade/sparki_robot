


#include<SparkiSLAM.h>
#include<Sparki.h>


const int angleStep = 5;
const int minAngle = -85;
const int maxAngle = 85;
int angle;

void setup(){
  angle = minAngle;
}

void loop(){
  SparkiSLAM::processSerial();
  
  moveServo();
  int distance = sparki.ping_single();
  if (distance != -1){
    SparkiSLAM::sendRange(angle, distance);
  }
}


void moveServo(){
  static boolean up = true;
  
  if (angle > maxAngle){
    up = false;
  }
  else if(angle < minAngle){
    up = true;
  }
  
  if (up){
    angle += angleStep;
  }
  else{
    angle -= angleStep;
  }

  sparki.servo(angle);
  delay(35);
}  
  
