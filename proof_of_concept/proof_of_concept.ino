
#include <SPI.h>
#include <Ethernet.h>

#include <Wire.h>
#include <SM130.h> 

byte validTagId[] = {0x72, 0xF5, 0x5A, 0xBF}; // 72F55ABF
SM130 RFIDuino;

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup()
{
  
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Post Tenebras Lab Door Access Control");
  
  //if (Ethernet.begin(mac) == 0) {
  //  Serial.println("Failed to configure Ethernet using DHCP");
  //}
  
  // pin for the relay
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
 
  // pin for the door switch
 
 
  // use pin 5 and 6 for the RFID status
  RFIDuino.address = 0x30;
  RFIDuino.pinRESET = 5; 
  RFIDuino.pinDREADY = 6;
  // reset RFIDuino
  RFIDuino.reset();

  // read firmware version
  Serial.print("Version ");
  Serial.println(RFIDuino.getFirmwareVersion());
  
  // help
   RFIDuino.setAntennaPower(true);
   delay(100);
   RFIDuino.seekTag();
}


void loop()
{
  // from 0 to 775
  int lightSensor = analogRead(A1);
  Serial.print("light sensor = ");
  Serial.println(lightSensor);
  
  if(RFIDuino.available()) {
      byte* tagNum = RFIDuino.getTagNumber();
      //if(tagNum[0] == 0 && tagNum[1] == 0 && tagNum[2] == 0 && tagNum[3] == 0) 
      Serial.print("tag read :");
      Serial.print(tagNum[0], HEX);
      Serial.print(tagNum[1], HEX);
      Serial.print(tagNum[2], HEX);
      Serial.println(tagNum[3], HEX);
      Serial.print("tag = ");
      Serial.println(RFIDuino.getTagString());
      
      if(!memcmp(tagNum, validTagId, 4)) {
        Serial.println("tag is accepted");
        digitalWrite(7, HIGH);
        delay(300);
        digitalWrite(7, LOW);
      }
      else {
        Serial.println("tag is rejected");
      }
      delay(500);
      RFIDuino.seekTag();
  }
  
}
