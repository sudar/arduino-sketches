/*

Basic push button

Control a LED using a push button. If you hold the button, LED will glow.

Circuit: Connect a led at pin 13 and a push button to analog pin 7
*/

#define LED 13
#define BUTTON 7

//int val = 0;

void setup() {
 pinMode (LED, OUTPUT); 
 pinMode (BUTTON, INPUT);
}

void loop() {
  
  digitalWrite(LED, digitalRead(BUTTON));
  
}

