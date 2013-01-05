/**
 * Two Arduinos communicating through software serial
 * 
 * - Connect A0(14) of first Arduino to A1(15) of second Arduino
 * - Connect A1(15) of first Arduino to A0(14) of second Arduino
 * - Connect the Gnd of both the Arduino together
 *
 * - Upload the same program to two Arduino's. 
 * - If you type a character in one serial monitor, it should appear in the second one
 *
 * Copyright 2012  Sudar Muthu  (http://sudarmuthu.com)
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Sudar <http://sudarmuthu.com> wrote this file. 
 * As long as you retain this notice you can do whatever you want with this stuff.
 * If we ever meet and you are overcome with gratitude,
 * feel free to express your feelings via beverage :)
 * ------------------------------------------------------------------------------
 */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(14, 15); // RX, TX

char serialVal;
char mySerialVal;

void setup() {  
    Serial.begin(115200);
    mySerial.begin(115200);

}

void loop() {
    if (mySerial.available()) {
        Serial.write(mySerial.read()); 
    }

    if (Serial.available()) {
        mySerial.write(Serial.read()); 
    }
}
