#include "heltec.h" 
#include "images.h"

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packet;

void LoRaData(int packetSize, String packet)
{
  
  String packSize = String(packetSize,DEC);
  String rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 11 , "Received "+ packSize + " bytes");
  //Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  int i = packet.indexOf(':');
  Heltec.display->drawStringMaxWidth(0 , 22 , 128, packet.substring(0, i));
  String sub = packet.substring(i+1);
  i = sub.indexOf(':');

  /*Serial.println(i);
  Serial.println(sub);
  Serial.println(sub.substring(0,i));
  Serial.println(sub.substring(i+1));*/
  
  if(i > 0)
    Heltec.display->drawString(0 , 33, sub.substring(0, i));
  Heltec.display->drawStringMaxWidth(0,44,128, sub.substring(i+1));

  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void printData(int packetSize) 
{
  packet ="";
  for (int i = 0; i < packetSize; i++) 
  { 
    packet += (char) LoRa.read(); 
  }
  
  LoRaData(packetSize, packet);
}

void setup() { 
  
 
  
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  initDisplay();
  
  delay(1500);

  
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  
  delay(1000);
  
  LoRa.receive();
}

void loop() 
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  { 
    printData(packetSize);  
  }
  delay(10);
}
