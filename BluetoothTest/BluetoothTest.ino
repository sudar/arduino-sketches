/*
simple LED test using Bluetooth
*/

#include <SoftwareSerial.h>

int val;         // variable to receive data from the serial port
char val2;
SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
    Serial.begin(115200);       // start serial communication to computer
    mySerial.begin(115200); // start serial communication to bluetooth at 115200bps  

    Serial.println("Ready");
}
 
void loop() 
{
    if( mySerial.available()) {      // if data is available to read
        //Serial.write(mySerial.read()); 
        val = mySerial.read();         // read it and store it in 'val'
        switch (val) {
            case 49:
                Serial.println("Front");
                break;
            case 50:
                Serial.println("Left");
                break;
            case 51:
                Serial.println("Right");
                break;
            case 52:
                Serial.println("Back");
                break;
            default:
                Serial.println("Brake");
                break;
        }
        //Serial.write(val);Serial.println();
        Serial.print(val, DEC);Serial.println();
        //mySerial.write(val);
    }

    if (Serial.available()) {
        val2 = Serial.read();
        mySerial.write(val2);
        Serial.write(val2);
    }
}
