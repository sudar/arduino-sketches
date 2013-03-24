/*
 Blinks two LED's alternatively

 Circuit: Connect two led's one in pin 13 and the other in pin 12

 */

int ledPin1 =  13;    // LED connected to digital pin 13
int ledPin2 =  12;    // LED connected to digital pin 12

// The setup() method runs once, when the sketch starts

void setup()   {                
  // initialize the digital pin as an output:
  pinMode(ledPin1, OUTPUT);     
  pinMode(ledPin2, OUTPUT);       
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  digitalWrite(ledPin1, HIGH);   // set the frist LED on
  digitalWrite(ledPin2, LOW);    // set the second LED off
  
  delay(1000);                  // wait for a second
  
  digitalWrite(ledPin1, LOW);    // set the first LED off
  digitalWrite(ledPin2, HIGH);   // set the second LED on    
  
  delay(1000);                  // wait for a second
}
