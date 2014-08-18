



#include<SparkiSLAM.h>

namespace SparkiSLAM
{
    void sendRange(int angle, int distance){
        String message = "r ";
        message = message + String(angle) + " " + String(distance);
        Serial.println(message);
    }
    
    void setTwist(float v, float omega){
        int leftMotorSpeed = int((v - TRACK_WIDTH_CM/400.0 * omega)*SPEED_CONVERSION);
        int rightMotorSpeed = int((v + TRACK_WIDTH_CM/400.0 * omega)*SPEED_CONVERSION);
        sparki.motorRotate(MOTOR_LEFT, -sgn(leftMotorSpeed), abs(leftMotorSpeed), ULONG_MAX);
        sparki.motorRotate(MOTOR_RIGHT, sgn(rightMotorSpeed), abs(rightMotorSpeed), ULONG_MAX);
    }
    
    void processSerial(){
        static char buffer[BUFFER_SIZE];
        static byte activeIndex = 0;
        
        byte messageLength = -1;
        
        while(Serial.available() > 0){
            //Get one message
            while(activeIndex < BUFFER_SIZE){
                byte c = Serial.read();
                if (c != '\n'){
                    buffer[activeIndex] = c;
                    activeIndex++;
                    if (activeIndex == BUFFER_SIZE){
                        Serial.println('o');
                        return;
                    }
                }
                else{
                    messageLength = activeIndex;
                    activeIndex = 0;
                    break;
                }
            }
            
            //Return if received message is incomplete
            if (messageLength == -1){
                return;
            }
            
            //Get data from message
            String inputString = String(buffer);
            inputString = inputString.substring(0, messageLength);
            char type = inputString.charAt(0);
            char option = inputString.charAt(2);
            String data = inputString.substring(3);
            data.trim();
            
            switch (type){
                case 'U':
                    switch (option){
                        case 'S':
                            sparki.moveStop();
                            break;
                        case 'V':
                            {
                            byte spaceIndex = data.indexOf(' ', 0);
                            String v_string = data.substring(0, spaceIndex+1);
                            char v_buffer[v_string.length()];
                            v_string.toCharArray(v_buffer, v_string.length());
                            float v = atof(v_buffer);
                            String o_string = data.substring(spaceIndex+1, data.length());
                            char o_buffer[o_string.length()+1];
                            o_string.toCharArray(o_buffer, o_string.length()+1);
                            float omega = atof(o_buffer);
                            SparkiSLAM::setTwist(v, omega);
                            
                            Serial.println(o_string);
                            Serial.println(o_buffer);
                            Serial.println(omega);
                            break;
                            }
                        default:
                            Serial.println('e');
                    }
                    break;
                default:
                    Serial.println('e');
            }
        }
    } 
    
    static inline int8_t sgn(int val) {
        if (val < 0) return -1;
        if (val == 0) return 0;
        return 1;
    }
        

}