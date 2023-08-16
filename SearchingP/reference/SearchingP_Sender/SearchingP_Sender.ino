#include "heltec.h"
#include "image.h"

#define BAND    433E6
#define TINY_GSM_MODEM_SIM808


#define SerialMon Serial
#define SerialAT  Serial2

#define TINY_GSM_DEBUG SerialMon
#define DUMP_AT_COMMANDS

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "3HK.IOT";

//https://thingspeak.com/channels/360216
const char host[] = "api.thingspeak.com";
const char publicKey[]  = "Y0RND6A62N0TTRSO";
const char privateKey[] = "O6OR0HHKBZLPA430";
const char resource[] = "/update?api_key=O6OR0HHKBZLPA430&field1=23&field2=115";
const int  port = 80;
int counter;
bool nbiotConnected = false;
int csq = 99;
int sendDataCounter = 99;
int error = 999;

String ipAddress;
String cop;
String ccid;

float lat;
float lon;
float spd;
int alt;
int vsat;
int usat;

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);
HttpClient http(client, host, port);

void setup() {

  
  Heltec.begin(true /*DisplayEnable*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND /*long BAND*/);

  initDisplay(); 
  delay(200);

  SerialAT.begin(9600, SERIAL_8N1, 17,13);
  delay(200);

    
  delay(1500);

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  
  delay(1000);
  
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // The txPin & rxPin can set to any output pin
  
  delay(3000);
  


}


  
void loop() 
{

  if(error > 100)
  {
    SerialMon.println(F("Initializing modem..."));
    Heltec.display->println("Initializing modem...");
    modem.restart();
    delay(500);
    String modemInfo = modem.getModemInfo();
    SerialMon.print(F("Modem: "));
    SerialMon.println(modemInfo);
    Heltec.display->println(modemInfo);
    delay(500);
    error = 0;
  }
  
  digitalWrite(LED, HIGH);
  csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);

  delay(200);
 
  String modemInfo = modem.getModemInfo();
  DBG("Modem:", modemInfo);
  
  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  DBG("CSQ:", csq);
  DBG("nbiot:", nbiotConnected);
  
  if((csq < 99) && (nbiotConnected == false))
  {
    ccid = modem.getSimCCID();
    DBG("CCID:", ccid);

    cop = modem.getOperator();
    DBG("Operator:", cop);

    modem.nbiotConnect("3HK.IOT");
    delay(3000);
  
    ipAddress = modem.getLocalIP();
    DBG("IP:", ipAddress);

    nbiotConnected = true;
    delay(3000);
  }
  else if(csq == 99)
  {
    nbiotConnected = false;
    error++;
    DBG("Error:", error);
    delay(1000);
  }

  modem.enableGPS();
  
  counter++;
  digitalWrite(LED, counter % 2); 
  String gps_raw = modem.getGPSraw();

  DBG("GPS raw data:", gps_raw);
  
  modem.getGPS(&lat, &lon,&spd, &alt, &vsat, &usat);
  
  SerialMon.print(lat,6);
  SerialMon.print(",");
  SerialMon.print(lon,6);
  SerialMon.print(",spd:");
  SerialMon.print(spd);
  SerialMon.print(",alt:");
  SerialMon.print(alt);
  SerialMon.print(",vsat:");
  SerialMon.print(vsat);
  SerialMon.print(",usat:");
  SerialMon.println(usat);
  delay(200);
  
  loraSend(String("counter, lat, lon, speed:") +
           String(counter) +  "," +
           String(lat,4) + "," +
           String(lon,4) + "," +
           String(spd) + ":" +
           String("CSQ:") + String(csq));        
  delay(200);
  
  Heltec.display->clear();
  displayGPS(lat, lon, spd, alt, vsat, usat);

  csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);
  displayCSQ(csq);
  Heltec.display->display();
    
  delay(200);
  
  sendDataCounter++;
  DBG("Send Counter:", sendDataCounter);
  DBG("nbiot:",nbiotConnected);
  
  if((sendDataCounter > 14) && nbiotConnected)
  {
    send_iot_data(host, privateKey, lat, lon, spd, alt, vsat, usat);
    sendDataCounter = 0;
  }

  delay(1000);
}

void send_iot_data(const char * host, const char * privateKey, float lat, float lon, float spd, int alt, int vsat, int usat)
{
  Serial.print("connecting to ");
  Serial.println(host);
  
  
  // We now create a URI for the request
  String url;
  url += "/update";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += String(lat, 6);
  url += "&field2=";
  url += String(lon, 6);
  url += "&field4=";
  url += String(spd, 6);
  url += "&field5=";
  url += alt;
  url += "&field6=";
  url += vsat;
  url += "&field7=";
  url += usat;  
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  int err = http.get(url);
  if (err != 0) 
  {
    SerialMon.println(F("failed to connect"));
    delay(10000);
    return;
  }

  delay(1000);
  http.stop();
  SerialMon.println(F("Server disconnected"));
  
}

void displayGPS(float lat, float lon, float spd, int alt, int vsat, int usat)
{

  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(0, 10, String(counter));
  Heltec.display->drawString(0, 20, String(lat));
  Heltec.display->drawString(30, 20, String(lon));
  Heltec.display->drawString(0, 30, String(spd));
  Heltec.display->drawString(0, 40, String(vsat));
  Heltec.display->drawString(30, 40, String(usat));
  
}

void displayCSQ(int csq)
{
  Heltec.display->drawString(0, 50, "Signal: ");
  Heltec.display->drawString(40, 50, String(csq));
}

void loraSend(String str)
{
   LoRa.beginPacket();
   LoRa.print(str);
   LoRa.endPacket();  
}
