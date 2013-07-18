
#include <SPI.h>
#include <Ethernet.h>
#include <RTClib.h>

#include <Wire.h>
#include <SM130.h> 

RTC_DS1307 RTC;
int rtc[7];
byte validTagId[] = {0x72, 0xF5, 0x5A, 0xBF}; // 72F55ABF
SM130 RFIDuino;

// Enter a MAC address for your controller below.
//Newer Ethernet shields have a MAC address printed on a sticker on the shield
//
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 81, 177 }; // need to be changed to a free fixed ip
byte gateway[] = { 192, 168, 81, 1 }; // rewrite it to .80.1 when ip will be chosen
// the subnet:
byte subnet[] = { 255, 255, 255, 0 };

EthernetServer webServer = EthernetServer(80);
byte* tagNum;
bool tagPresent;

// PINs variables
//
int RELAY = 7;
int SWITCH_TOP = 8;
int SWITCH_LOCKER = 9;
int RED = 3;
int RESET = 5;
int READY = 6;

// Time counter check time duration longer than loop execution
// 
unsigned long OPEN_DOOR_TIMEOUT = 15000; // door switch must detect changes; if not, send an alert (door is open even if it "look closed")
unsigned long TimerA // door timeout timer

void setup()
{
  tagPresent = false;
  
  // pin for the relay
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  
  // pin for the door switch
  pinMode(SWITCH_TOP, INPUT); // top door's switch
  pinMode(SWITCH_LOCKER, INPUT); // locker door's switch

  // pin for the LED
  pinMode(RED, OUTPUT);
  digitalWrite(RED, LOW);
  
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Post Tenebras Lab Door Access Control");
  
  Serial.print("Setting up RTC clock...");
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println(" RTC is NOT running! Using compilation time as actual time.");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  else {
    Serial.println(" OK");
  }
  
  // setup ethernet
  Serial.print("Setting up ethernet...");
  if (Ethernet.begin(mac, ip, gateway, subnet) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  webServer.begin();
 Serial.println("  OK");
 
 Serial.print("Setting up RFID reader... ");
  // use pin 5 and 6 for the RFID status
  RFIDuino.address = 0x30;
  RFIDuino.pinRESET = RESET; 
  RFIDuino.pinDREADY = READY;
  // reset RFIDuino
  RFIDuino.reset();

  // read firmware version
  Serial.print("version ");
  Serial.print(RFIDuino.getFirmwareVersion());
  
  // help
   RFIDuino.setAntennaPower(true);
   delay(100);
   RFIDuino.seekTag();
   Serial.println("  OK");
}

void loop()
{
  // from 0 to 775
  int lightSensor = analogRead(A1);
  //Serial.print("light sensor = ");
  //Serial.println(lightSensor);

  // One switch LOW and other HIGN mean that the door is closed AND unlocked
  if( digitalRead(SWITCH_TOP) == LOW && digitalRead(SWITCH_LOCKER) == HIGH ) {
      digitalWrite(RED, HIGH)
  }

  if(RFIDuino.available()) {
      tagNum = RFIDuino.getTagNumber();
      tagPresent = true;
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

        if( digitalRead(9) == LOW ) {
           digitalWrite(RELAY, HIGH);
           delay(300);
           digitalWrite(RELAY, LOW);
        }
      }
      else {
        Serial.println("tag is rejected");
      }
      delay(500);
      RFIDuino.seekTag();
  }

  EthernetClient client = webServer.available();
  if (client) {
    Serial.println("web client connected");
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connnection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    // add a meta refresh tag, so the browser pulls again every 5 seconds:
    client.println("<meta http-equiv=\"refresh\" content=\"5\">");
    client.print("lightSensor = ");
    client.print(lightSensor);
    client.println("<br />");
    
    client.print("door switch 1 = ");
    client.print(digitalRead(SWITCH_TOP) == HIGH ? "OPEN" : "CLOSE");
    client.println("<br />");
    
    client.print("door switch 2 = ");
    client.print(digitalRead(SWITCH_LOCKER) == HIGH ? "OPEN" : "CLOSE");
    client.println("<br />");
    
    if(tagPresent) {
      client.print("tag read :");
      client.print(tagNum[0], HEX);
      client.print(tagNum[1], HEX);
      client.print(tagNum[2], HEX);
      client.println(tagNum[3], HEX);
      client.println("<br />");
      tagPresent = false;
   }
   
   
   client.print("rtc time :");
   DateTime now = RTC.now();
    client.print(now.year(), DEC);
    client.print('/');
    client.print(now.month(), DEC);
    client.print('/');
    client.print(now.day(), DEC);
    client.print(' ');
    client.print(now.hour(), DEC);
    client.print(':');
    client.print(now.minute(), DEC);
    client.print(':');
    client.print(now.second(), DEC);
    client.println();
  client.println("<br />");
  
  client.println("</html>");
  client.stop();
  }
  
}
