


#ifndef _SparkiSerial_h_
#define _SparkiSerial_h_

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <sparki.h>
#include <limits.h>
#include <stdlib.h>

#define BUFFER_SIZE 32

class SparkiSerial
{
    public:
        SparkiSerial();
        void begin(void);
        int readOneMessage(char *buffer, size_t length);
        void doAllMessages();
        void processMessage(char *buffer, int messageLength);
        
        //Send Messages
        float *sendAccelerometer(char option);
        int sendDistance(char option);
        int *sendIRReflect(char option);
        int *sendLightTransitors(char option);
        float *sendMagnet(char option);
        int *sendRemote(char option);
        
    private:
        //Process Commands
        void processBeep(char option, String data);
        void processIREmit(char option, String data);
        void processGripper(char option, String data);
        void processLED(char option, String data);
        void processServo(char option, String data);
        void processMotors(char option, String data);
    

        
        //Motors
        void moveVO(String data);
        void moveLR(String data);
        void moveForward();
        void moveForward(float cm);
        void moveBackward();
        void moveBackward(float cm);
        void turnLeft();
        void turnRight();
        void turnRad();
        void setSpeed();
        
        //IR Communication
        void sendByte(byte data);
        void sendCharArray(char *data);
        
        //Data processing
        int *stringToInt(String data);
        float *stringToFloat(String data);
        
};

extern SparkiSerial sparkiserial;

#endif