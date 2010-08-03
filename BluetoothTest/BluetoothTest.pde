/*
simple LED test using Bluetooth
*/

char val;         // variable to receive data from the serial port
int ledPin = 12;  // LED connected to pin 12 (on-board LED)
int toggle = 0;
int outpin = 13;

void setup()
{
  pinMode(12, OUTPUT);  // pin 13 (on-board LED) as OUTPUT  
  pinMode(13, OUTPUT);  // pin 13 (on-board LED) as OUTPUT
  Serial.begin(115200);       // start serial communication at 115200bps  
}
 
void loop() {

 if( Serial.available() )       // if data is available to read
  {
    val = Serial.read();         // read it and store it in 'val'
 
 if (val == '0') {
    digitalWrite(outpin, HIGH);    // turn Off pin 13 off     
 } else {   
    digitalWrite(outpin, LOW);    // turn Off pin 13 off     
 }
     
 if (toggle == 0) {
    digitalWrite(ledPin, HIGH);    // turn Off pin 13 off
   toggle = 1;
 } else {  
    digitalWrite(ledPin, LOW);    // turn Off pin 13 off
   toggle = 0;  
 }
 
 Serial.print(val);
  }
}
