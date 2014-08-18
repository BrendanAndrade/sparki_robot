



#ifndef _SparkiSLAM_h_
#define _SparkiSLAM_h_

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include<sparki.h>
#include<limits.h>

#define BUFFER_SIZE 32

#define SPEED_CONVERSION 3724.0 //(m/s) per Sparki speed unit

namespace SparkiSLAM
{
    void sendRange(int angle, int distance);
    void setTwist(float v, float omega);
    void processSerial();
    
    static inline int8_t sgn(int val);
}

#endif