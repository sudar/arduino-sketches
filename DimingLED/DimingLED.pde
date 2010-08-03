/*
Diming LED

Makes a LED to fade in and fade out.

Circuit: Connect a LED to analog pin # 7

*/

#define LED 9
int i = 0;

void setup() {
 pinMode(LED, OUTPUT); 
}


void loop() {
  
  //Fade in
 for (i = 0; i <  255; i++) {
   analogWrite(LED, i);
   delay(10);
 }  
 
 //Fade out
 for (i = 255; i > 0 ; i--) {
  analogWrite(LED, i);
  delay(10); 
 }
 
}
