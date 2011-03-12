/**
 * Arduino sketche to read button press from a USB mouse
 *
 * Connect your USB Host shield
 * Based on the examples from USB Host Shield by Circuits @ home
 */
 
//#include <Spi.h>
#include <Max3421e.h>
#include <Usb.h>
 
#define DEVADDR 1
#define CONFVALUE 1

#define ftl(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))  //float to long conversion

void setup();
void loop();
 
MAX3421E Max;
USB Usb;

void setup() {
  Max.powerOn();
  Serial.begin( 9600 );
  Serial.println("Start");
  delay( 500 );
}

void loop() {
    byte rcode;
    //delay( 10 );
    Max.Task();
    Usb.Task();
    
    if( Usb.getUsbTaskState() == USB_STATE_CONFIGURING ) {  
        mouse_init();
    }//if( Usb.getUsbTaskState() == USB_STATE_CONFIGURING...
    
    if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {  //poll the keyboard  
        rcode = mouse_poll();
        if( rcode ) {
          Serial.print("Mouse Poll Error: ");
          Serial.println( rcode, HEX );
        }//if( rcode...
    }//if( Usb.getUsbTaskState() == USB_STATE_RUNNING...
}

/* Initialize mouse */
void mouse_init( void ) {
  Serial.println("Mouse Init");
 byte rcode = 0;  //return code
  /**/
  Usb.setDevTableEntry( 1, Usb.getDevTableEntry( 0,0 ) );              //copy device 0 endpoint information to device 1
  /* Configure device */
  rcode = Usb.setConf( DEVADDR, 0, CONFVALUE );                    
  if( rcode ) {
    Serial.print("Error configuring mouse. Return code : ");
    Serial.println( rcode, HEX );
    while(1);  //stop
  }//if( rcode...
  Usb.setUsbTaskState( USB_STATE_RUNNING );
  return;
}

/* Poll mouse using Get Report and fill arm data structure */
byte mouse_poll( void ) {
  Serial.println("Mouse poll");  
  byte rcode;
  char buf[ 4 ];                //mouse buffer
  static uint16_t delay = 500;  //delay before park

    /* poll mouse */
    rcode = Usb.getReport( DEVADDR, 0, 4, 0, 1, 0, buf );
    if( rcode ) {  //error
      return( rcode );
    }
    
    Serial.println("buffer value");
    Serial.println(buf[0]);
      
    switch( buf[ 0 ] ) {  //read buttons
      case 0x00:            //no buttons pressed
        // armdata.z_coord += ( buf[ 3 ] * -2 ) ;
        break;
      case 0x01:            //button 1 pressed. 
        Serial.println("Button 1 is pressed");
        break;
      case 0x02:           //button 2 pressed. 
        Serial.println("Button 2 is pressed");
        break;
      case 0x04:          //wheel button pressed. 
        Serial.println("Wheel button is pressed");
        break;
      case 0x07:          //all 3 buttons pressed.
        Serial.println("All 3 button is pressed");
        break;        
    }//switch( buf[ 0 ...
}
