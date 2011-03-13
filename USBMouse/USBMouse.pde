/**
 * Comunicate with a USB Mouse via interrupt endpoint.
 * You need a USB Host Shield, USB Mouse and an Arduino board
 * You also need the USB_Host_Shield library available at https://github.com/felis/USB_Host_Shield 
 *
 * (Assumes EP1 as interrupt IN ep)
 *
 * Code based on the sketch by Circuits @ home - http://www.circuitsathome.com/communicating-arduino-with-hid-devices-part-1
 */

#include <Max3421e.h>
#include <Usb.h>
 
#define DEVADDR 1
#define CONFVALUE 1
#define EP_MAXPKTSIZE 5
EP_RECORD ep_record[ 2 ];  //endpoint record structure for the mouse  
 
void setup();
void loop();
 
MAX3421E Max;
USB Usb;
 
void setup() {
    Serial.begin( 115200 );
    Serial.println("Start");
    Max.powerOn();
    delay( 200 );
}
 
void loop() {
    byte rcode;
    Max.Task();
    Usb.Task();

    if( Usb.getUsbTaskState() == USB_STATE_CONFIGURING ) {
        mouse1_init();
    }//if( Usb.getUsbTaskState() == USB_STATE_CONFIGURING...

    if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {  //poll the mouse
        rcode = mouse1_poll();
        if( rcode ) {
            Serial.print("Mouse Poll Error: ");
            Serial.println( rcode, HEX );
        }//if( rcode...
    }//if( Usb.getUsbTaskState() == USB_STATE_RUNNING...
}

/* Initialize mouse */
void mouse1_init( void ) {
    byte rcode = 0;  //return code
    byte tmpdata;
    byte* byte_ptr = &tmpdata;

    /**/
    ep_record[ 0 ] = *( Usb.getDevTableEntry( 0,0 ));  //copy endpoint 0 parameters
    ep_record[ 1 ].MaxPktSize = EP_MAXPKTSIZE;
    ep_record[ 1 ].sndToggle = bmSNDTOG0;
    ep_record[ 1 ].rcvToggle = bmRCVTOG0;
    Usb.setDevTableEntry( 1, ep_record );              //plug kbd.endpoint parameters to devtable

    /* Configure device */
    rcode = Usb.setConf( DEVADDR, 0, CONFVALUE );
    if( rcode ) {
        Serial.print("Error configuring mouse. Return code : ");
        Serial.println( rcode, HEX );
        while(1);  //stop
    }//if( rcode...

    rcode = Usb.getIdle( DEVADDR, 0, 0, 0, (char *)byte_ptr );
    if( rcode ) {
        Serial.print("Get Idle error. Return code : ");
        Serial.println( rcode, HEX );
        while(1);  //stop
    }

    Serial.print("Idle Rate: ");
    Serial.print(( tmpdata * 4 ), DEC );        //rate is returned in multiples of 4ms
    Serial.println(" ms");

    tmpdata = 0;
    rcode = Usb.setIdle( DEVADDR, 0, 0, 0, tmpdata );
    if( rcode ) {
        Serial.print("Set Idle error. Return code : ");
        Serial.println( rcode, HEX );
        while(1);  //stop
    }

    Usb.setUsbTaskState( USB_STATE_RUNNING );
    return;
}

/* Poll mouse via interrupt endpoint and print result */
/* assumes EP1 as interrupt endpoint                  */
byte mouse1_poll( void ) {
    byte rcode,i;
    char buf[ 4 ] = { 0 };                          //mouse report buffer
    /* poll mouse */
    rcode = Usb.inTransfer( DEVADDR, 1, 4, buf, 1 );  //

    if( rcode ) {  //error
        if( rcode == 0x04 ) {  //NAK
            rcode = 0;
        }
        return( rcode );
    }

    /* print buffer */
    if( buf[ 0 ] & 0x01 ) {
        Serial.println("Button1 pressed ");
    }
    if( buf[ 0 ] & 0x02 ) {
        Serial.println("Button2 pressed ");
    }
    if( buf[ 0 ] & 0x04 ) {
        Serial.println("Button3 pressed ");
    }

    /* This will print the x,y and wheel co-ordinates as well
    Serial.println("");
    Serial.print("X-axis: ");
    Serial.println( buf[ 1 ], DEC);
    Serial.print("Y-axis: ");
    Serial.println( buf[ 2 ], DEC);
    Serial.print("Wheel: ");
    Serial.println( buf[ 3 ], DEC);
    Serial.println("");
    */
    return( rcode );
}
