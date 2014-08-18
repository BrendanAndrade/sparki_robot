/*

*/

#include <SparkiSerial.h>

SparkiSerial sparkiserial;

SparkiSerial::SparkiSerial(){
    begin();
}

void SparkiSerial::begin(){
    sparki.begin();
}

/******* Message Handling *******/
//Read all data from serial port and execute received commands
void SparkiSerial::doAllMessages(){
    static char buffer[BUFFER_SIZE];
    
    if (Serial.available() == 0){
        return;
    }

    while(Serial.available() > 0){

        int messageLength = readOneMessage(buffer, BUFFER_SIZE);
        if (messageLength == -1){
            break;
        }

        //Process message
        processMessage(buffer, messageLength);   
    }
}
    

int SparkiSerial::readOneMessage(char *buffer, size_t length){
    static int activeIndex = 0;

    while(activeIndex < length){
        byte c = Serial.read();
        if (c != '\n'){
            buffer[activeIndex] = c;
            activeIndex++;
        }
        else{
            int fillSize = activeIndex;
            activeIndex = 0;
            return fillSize;
        }
    } 
    return -1;
}

void SparkiSerial::processMessage(char *buffer, int messageLength){
    String inputString = String(buffer);
    inputString = inputString.substring(0, messageLength);
    char inputArray[2];
    byte lastSpace = 0;
    for(byte i = 0; i < 2; i++){
        byte spaceIndex = inputString.indexOf(' ', lastSpace+1);
        String digit = inputString.substring(lastSpace, spaceIndex);
        digit.trim();
        inputArray[i] = digit.charAt(0);
        lastSpace = spaceIndex;
    }
    String data = inputString.substring(lastSpace);
    data.trim();
    
    switch (inputArray[0]){
        case 'A':
            sendAccelerometer(inputArray[1]);
            break;
        
        case 'B':
            processBeep(inputArray[1], data);
            break;
        /*
        case 'C':
            processIREmit(inputArray[1], data);
            break;
        */
        case 'D':
            sendDistance(inputArray[1]);
            break;
        /*
        case 'G':
            processGripper(inputArray[1], data);
            break;
        */
        case 'I':
            sendRemote(inputArray[1]);
            break;
        
        case 'L':
            processLED(inputArray[1], data);
            break;
            
        case 'M':
            sendMagnet(inputArray[1]);
            break;
        
        case 'R':
            sendIRReflect(inputArray[1]);
            break;
        
        case 'S':
            processServo(inputArray[1], data);
            break;
        
        case 'T':
            sendLightTransitors(inputArray[1]);
            break;
        /*
        case 'U':
            processMotors(inputArray[1], data);
            break;            
        */
        default:
            Serial.println('e');
    }
}

void SparkiSerial::processBeep(char option, String data){
    if (data.length() == 0){
        sparki.beep();
    }
    else{
        int *note = stringToInt(data);;
        sparki.beep(note[0]);
    }
}

void SparkiSerial::processGripper(char option, String data){
    if (data.length() == 0){
        switch (option) {
            case 'O':
                sparki.gripperOpen();
                break;
            case 'C':
                sparki.gripperClose();
                break;
            case 'S':
                sparki.gripperStop();
                break;
            default:
                Serial.println("e");
        }
        return;
    }
    float *val = stringToFloat(data);
    float cm = val[0];
    switch (option){
        case 'O':
            sparki.gripperOpen(cm);
            break;
        case 'C':
            sparki.gripperClose(cm);
            break;
        case 'S':
            sparki.gripperStop();
            break;
        default:
            Serial.println("e");
    }
}

void SparkiSerial::processLED(char option, String data){
    int *colors = stringToInt(data);
    sparki.RGB(colors[0], colors[1], colors[2]);
}

void SparkiSerial::processServo(char option, String data){
    int *angle = stringToInt(data);
    sparki.servo(angle[0]);
}

/******* Send Data *******/

float *SparkiSerial::sendAccelerometer(char option){
    String message = "a ";
    
    float xyz[3];
    
    switch (option){
        case 'X':
            {
            xyz[0] = sparki.accelX();
            char buffer[8];
            dtostrf(xyz[0], 8, 5, buffer);
            message.concat(buffer);
            break;
            }
        case 'Y':
            {
            xyz[1] = sparki.accelY();
            char buffer[8];
            dtostrf(xyz[1], 8, 5, buffer);
            message.concat(buffer);
            break;
            }
        case 'Z':
            {
            xyz[2] = sparki.accelZ();
            char buffer[8];
            dtostrf(xyz[2], 8, 5, buffer);
            message.concat(buffer);
            break;
            }
        case 'A':
            {
            xyz[0] = sparki.accelX();
            xyz[1] = sparki.accelY();
            xyz[2] = sparki.accelZ();
            for (byte i = 0; i < 3; i++){
                char buffer[8];
                dtostrf(xyz[i], 8, 5, buffer);
                message.concat(buffer);
                message.concat(' ');
            }
            break;
            }
        default:
            message = "e";  
    }
    
    Serial.println(message);
    return xyz;
}

int SparkiSerial::sendDistance(char option){
    String message = "d ";
    int cm;
    switch (option){
    case 'A':
        cm = sparki.ping();
        message.concat(cm);
        break;
    case 'S':
        cm = sparki.ping_single();
        message.concat(cm);
        break;
    default:
        message = "e";
    }
    Serial.println(message);
    return cm;
}

int *SparkiSerial::sendIRReflect(char option){
    String message = "r ";
    int ir[5];
    switch (option){
        case 'A':
            ir[0] = sparki.edgeLeft();
            ir[1] = sparki.lineLeft();
            ir[2] = sparki.lineCenter();
            ir[3] = sparki.lineRight();
            ir[4] = sparki.edgeRight();
            for (byte i = 0; i < 5; i++){
                message.concat(ir[i]);
                message.concat(" ");
            }
            break;
        case 'E':
            ir[0] = sparki.edgeLeft();
            ir[4] = sparki.edgeRight();
            for (byte i = 0; i < 5; i+=4){
                message.concat(ir[i]);
                message.concat(" ");
            }
            break;
        case 'L':
            ir[1] = sparki.lineLeft();
            ir[2] = sparki.lineCenter();
            ir[3] = sparki.lineRight();
            for (byte i = 1; i<4; i++){
                message.concat(ir[i]);
                message.concat(" ");
            }
            break;
        case '1':
            ir[0] = sparki.edgeLeft();
            message.concat(ir[0]);
            break;
        case '2':
            ir[1] = sparki.lineLeft();
            message.concat(ir[1]);
            break;
        case '3':
            ir[2] = sparki.lineCenter();
            message.concat(ir[2]);
            break;
        case '4':
            ir[3] = sparki.lineRight();
            message.concat(ir[3]);
            break;
        case '5':
            ir[4] = sparki.edgeRight();
            message.concat(ir[4]);
            break;
        default:
            message = "e";
    }
    Serial.println(message);
    return ir;
}

int *SparkiSerial::sendLightTransitors(char option){
    String message = "t ";
    int lcr[3];
    switch (option){
        case 'L':
            lcr[0] = sparki.lightLeft();
            message.concat(lcr[0]);
            break;
        case 'C':
            lcr[1] = sparki.lightCenter();
            message.concat(lcr[1]);
            break;
        case 'R':
            lcr[2] = sparki.lightRight();
            message.concat(lcr[2]);
            break;
        case 'A':
            lcr[0] = sparki.lightLeft();
            lcr[1] = sparki.lightCenter();
            lcr[2] = sparki.lightRight();
            for (byte i = 0; i < 3; i++){
                message.concat(lcr[i]);
                message.concat(" ");
            }
            break;
        default:
            message = "e";
    }
    Serial.println(message);
    return lcr;
}

float *SparkiSerial::sendMagnet(char option){
    String message = "m ";
    
    float xyz[3];
    
    switch (option){
        case 'X':
            {
            xyz[0] = sparki.magX();
            char buffer[8];
            dtostrf(xyz[0], 8, 2, buffer);
            message.concat(buffer);
            break;
            }
        case 'Y':
            {
            xyz[1] = sparki.magY();
            char buffer[8];
            dtostrf(xyz[1], 8, 2, buffer);
            message.concat(buffer);
            break;
            }
        case 'Z':
            {
            xyz[2] = sparki.magZ();
            char buffer[8];
            dtostrf(xyz[2], 8, 2, buffer);
            message.concat(buffer);
            break;
            }
        case 'A':
            {
            xyz[0] = sparki.magX();
            xyz[1] = sparki.magY();
            xyz[2] = sparki.magZ();
            for (byte i = 0; i < 3; i++){
                char buffer[8];
                dtostrf(xyz[i], 8, 2, buffer);
                message.concat(buffer);
                message.concat(' ');
            }
            break;
            }
        default:
            message = "e";  
    }
    
    Serial.println(message);
    return xyz;
}

int *SparkiSerial::sendRemote(char option){
    //This needs work seems not to be able to receive multiple messages.

    String message = "i ";
    
    int buffer[BUFFER_SIZE];
    
    switch (option){
        case 'A':
            {
            for (byte i = 0; i < BUFFER_SIZE; i++){
                int code = sparki.readIR();
                if (code == -1){
                    break;
                }
                buffer[i] = code;
                message.concat(code);
                message.concat(" ");
            }
            break;
            }
        case 'S':
            {
            int code = sparki.readIR();
            if (code == -1){
                break;
            }
            buffer[0] = code;
            message.concat(code);
            break;
            }
        default:
            {
            message = "e";
            }
    }
    
    Serial.println(message);
    return buffer;
}
    

/******* Data processing *******/
int *SparkiSerial::stringToInt(String data){
    data.trim();
    byte nSpaces = 0;
    for (byte i=0; i < data.length(); i++){
        nSpaces += (data[i] == ' ');
    }
    int output[nSpaces+1];
    byte startIndex = 0;
    for (byte i = 0; i < nSpaces; i++){
        byte spaceIndex = data.indexOf(' ', startIndex);
        String number = data.substring(startIndex, spaceIndex);
        startIndex = spaceIndex + 1;
        output[i] = number.toInt();

    }
    String number = data.substring(startIndex);
    output[nSpaces] = number.toInt();
    return output;
}

float *SparkiSerial::stringToFloat(String data){
    data.trim();
    byte nSpaces = 0;
    for (byte i=0; i < data.length(); i++){
        nSpaces += (data[i] == ' ');
    }
    float output[nSpaces+1];
    byte startIndex = 0;
    for (byte i = 0; i < nSpaces; i++){
        byte spaceIndex = data.indexOf(' ', startIndex);
        String number = data.substring(startIndex, spaceIndex);
        startIndex = spaceIndex + 1;
        char buffer[number.length()];
        number.toCharArray(buffer, number.length());
        output[i] = atof(buffer);
    }
    String number = data.substring(startIndex);
    char buffer[number.length()];
    number.toCharArray(buffer, number.length());
    output[nSpaces] = atof(buffer);
    return output;
}
        
