
Description :

Ce projet a pour but de Contrôler l'accès au local du Post Tenebras
Lab à l'aide d'un arduino.


...
Hardware :

 - Arduino
 - RFID SM130 
	http://www.sonmicro.com/en/index.php?option=com_content&view=article&id=57&Itemid=70
 - Grove base shield 
	http://www.seeedstudio.com/wiki/Grove_-_Base_Shield
 - Microswicth
	http://shop.boxtec.ch/microswitch-spdt-hinge-roller-lever-p-41015.html
 - Microphone
	http://shop.boxtec.ch/electronic-brick-sound-sensor-microphone-brick-p-40472.html
 - Relay 
	http://shop.boxtec.ch/twig-relay-board-p-40428.html
 - RTC DS1307
	http://shop.boxtec.ch/tiny-ds1307-module-eeprom-p-41033.html

Libraries :

la librairy RFIDuino (https://github.com/marcboon/RFIDuino.git) est requise par le lecteur RFID "sm130" 
la librairy ethernet (arduino)
la librairy RTClib https://github.com/adafruit/RTClib

Notes :
 - Attention, utiliser une alimentation externe, l'usb ne suffit pas 
