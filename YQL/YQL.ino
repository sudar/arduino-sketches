/*
  YQL client with JSON parsing
 
 This sketch connects to YQL using an Ethernet shield. It parses the JSON
 returned, and looks for the first title in the RSS Feed
 
 You can use the Arduino Ethernet shield, or the Adafruit Ethernet shield, 
 either one will work, as long as it's got a Wiznet Ethernet module on board.
 
 Uses the following libraries:

 - DHCP routines in the Ethernet library which is part of the Arduino core from version 1.0 beta 1
 - String library, which is part of the Arduino core from version 0019.  
 - aJson library for JSON parsing - https://github.com/interactive-matter/aJson
 - MemoryFree library for finding out how much free memory is left - https://github.com/sudar/memoryfree
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 Author:
 Sudar - <http://sudarmuthu.com> <http://hardwarefun.com>
 
 */
#include <SPI.h>
#include <Ethernet.h>
#include <aJSON.h>
#include <MemoryFree.h>

// function definitions
void connectToServer();
void URLEncodeAndSend(char * msg);
char* parseJson(char *jsonString);
void printFreeMemory(char* message);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };
IPAddress ip(10,0,0,9);

// initialize the EthernetClient instance:
EthernetClient client;

// YQL related 
char serverName[] = "query.yahooapis.com";  // Yahoo YQL URL
char URL[] = "GET /v1/public/yql?q=";
char YQLQuery[] = "select title from feed where url='http://sudarmuthu.com/feed' limit 1";
char Format[] = "&format=json&env=";
char Env[] = "store://datatables.org/alltableswithkeys";
char HTTPVersion[]="  HTTP/1.1";
char HOST[] = "HOST: query.yahooapis.com";

// Status related
String jsonResponse = "";
boolean jsonStarted = false;
boolean responseParsed = false;
short  bracesCount = 0;

void setup() 
{

    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    printFreeMemory("Before Connection");

    // attempt a DHCP connection:
    Serial.println(F("Attempting to get an IP address using DHCP:"));
    if (!Ethernet.begin(mac)) {
        // if DHCP fails, start with a hard-coded address:
        Serial.println(F("failed to get an IP address using DHCP, trying manually"));
        Ethernet.begin(mac, ip);
    }

    Serial.print(F("My IP:"));
    Serial.println(Ethernet.localIP());

    // connect to YQL Server
    connectToServer();

    printFreeMemory("After Setup");
}

void loop() 
{
    if (client.connected()) {
        if (client.available()) {

            // read incoming bytes:
            char inChar = client.read();

            Serial.print(inChar);
            // Extract only the JSON part of the response
            if (!jsonStarted && inChar == '{') {
                jsonStarted = true;
            }

            if (inChar == '{') {
                bracesCount++;
            }

            if (inChar == '}') {
                bracesCount--;
            }

            if (jsonStarted ) {
                jsonResponse += inChar;
            }

            if (bracesCount == 0) {
                jsonStarted = false;
            }
        } else {
            if (!responseParsed && jsonResponse.length() > 0) {
                printFreeMemory("Before parsing");

                // We have the json response, let's start to parse it
                jsonResponse.trim();

                Serial.println(jsonResponse);
                Serial.println(jsonResponse.length());

                char* jsonString = (char*) malloc(sizeof(char)*(jsonResponse.length()+1));
                jsonResponse.toCharArray(jsonString, jsonResponse.length() + 1);

                Serial.println(jsonString);

                char* value = parseJson(jsonString);

                if (value) {
                    Serial.print(F("Successfully Parsed: "));
                    Serial.println(value);
                } else {
                    Serial.print(F("There was some problem in parsing the JSON"));
                }

                responseParsed = true;

                printFreeMemory("After parsing");

                // We are done. Let's close the client request
                client.stop(); 
            }
        }   
    }
}

void connectToServer()
{
    // attempt to connect
    Serial.println(F("Connecting to server..."));

    if (client.connect(serverName, 80)) {
        Serial.println(F("Making HTTP request..."));
        // make HTTP GET request to YQL:
        client.print(URL);
        URLEncodeAndSend(YQLQuery);
        client.print(Format);
        URLEncodeAndSend(Env);
        client.println(HTTPVersion);
        client.println("HOST: query.yahooapis.com");
        client.println();
    }
}   

/**
 * Adapted from: http://www.icosaedro.it/apache/urlencode.c
 *   Sends a message to the server encoding if needs be
 */
void URLEncodeAndSend(char * msg)
{
    char *hex = "0123456789abcdef";
    int Numbertobeencoded=0;
    //Pass 1
    char *c=msg;

    while (*c!='\0'){
        if( ('a' <= *c && *c <= 'z')
                || ('A' <=* c && *c <= 'Z')
                || ('0' <= *c && *c <= '9') ) {
            client.print(*c);
#ifdef DEBUG
            Serial.print(*c);
#endif
        } else {
            client.print('%');
            client.print(hex[*c >> 4]);
            client.print(hex[*c & 15]);
#ifdef DEBUG
            Serial.print('%');
            Serial.print(hex[*c >> 4]);
            Serial.print(hex[*c & 15]);
#endif
        }
        c++;
    }
#ifdef DEBUG
    Serial.println("Done Sending !");
#endif
}

char* parseJson(char *jsonString) 
{
    char* value;

    aJsonObject* root = aJson.parse(jsonString);

    if (root != NULL) {
        //Serial.println("Parsed successfully 1 " );
        aJsonObject* query = aJson.getObjectItem(root, "query"); 

        if (query != NULL) {
            //Serial.println("Parsed successfully 2 " );
            aJsonObject* results = aJson.getObjectItem(query, "results"); 

            if (results != NULL) {
                //Serial.println("Parsed successfully 3 " );
                aJsonObject* item = aJson.getObjectItem(results, "item"); 

                if (item != NULL) {
                    //Serial.println("Parsed successfully 4 " );
                    aJsonObject* title = aJson.getObjectItem(item, "title"); 
                    
                    if (title != NULL) {
                        //Serial.println("Parsed successfully 5 " );
                        value = title->valuestring;
                    }
                }
            }
        }
    }

    if (value) {
        return value;
    } else {
        return NULL;
    }
}

/**
 * Print the Free memory along with a message in the Serial window
 */
void printFreeMemory(char* message)
{
    Serial.print(message);
    Serial.print(":\t");
    Serial.println(getFreeMemory());
}
