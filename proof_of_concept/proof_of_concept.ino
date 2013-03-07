
#include <Wire.h>
#include <SM130.h> 

byte validTagId[] = {0x72, 0xF5, 0x5A, 0xBF}; // 72F55ABF
SM130 RFIDuino;


void setup()
{
  
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Post Tenebras Lab Door Access Control");

  RFIDuino.address = 0x30;
  RFIDuino.pinRESET = 13;
  RFIDuino.pinDREADY = 12;
  // reset RFIDuino
  RFIDuino.reset();

  // read firmware version
  Serial.print("Version ");
  Serial.println(RFIDuino.getFirmwareVersion());
  
  // help
   RFIDuino.setAntennaPower(true);
   RFIDuino.seekTag();
}


void loop()
{
  
  
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
