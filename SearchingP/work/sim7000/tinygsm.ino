#include "heltec.h"

#define BAND    433E6
// #define TINY_GSM_MODEM_SIM808
#define TINY_GSM_MODEM_SIM7000


#define SerialMon Serial
#define SerialAT  Serial2

#define TINY_GSM_DEBUG SerialMon
#define DUMP_AT_COMMANDS

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>

// const char apn[]  = "3HK.IOT";
const char apn[]  = "mobile.lte.three.com.hk";
// const char apn[]  = "mobile.three.com.hk";

const int  port = 80;
bool nbiotConnected = false;
int csq = 99;
int error = 0;




TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient  mqtt(client);

void setup() {
  // put your setup code here, to run once:
  Heltec.begin(false /*DisplayEnable*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND /*long BAND*/);
  SerialAT.begin(9600, SERIAL_8N1, 17,13);
  delay(200);

  SerialMon.println(F("Initializing modem..."));
  modem.restart();
  delay(500);


  modem.setNetworkMode(38); //38-LTE, 13-GSM
  modem.setPreferredMode(3); //2-NB-IoT, 1-CAT, 3-GPRS
    
}

void loop() {


  if(error > 5)
  {
    SerialMon.println(F("Initializing modem..."));
    // Heltec.display->println("Initializing modem...");
    modem.restart();
    delay(500);
    String modemInfo = modem.getModemInfo();
    SerialMon.print(F("Modem: "));
    SerialMon.println(modemInfo);
    delay(500);
    error = 0;
  }

  // digitalWrite(LED, HIGH);
  csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);

  delay(200);

  
  if((csq < 99) && (nbiotConnected == false))
  {
    // ccid = modem.getSimCCID();
    // DBG("CCID:", ccid);

    // cop = modem.getOperator();
    // DBG("Operator:", cop);

    // // modem.nbiotConnect("3HK.IOT");
    // // delay(3000);
  
    // ipAddress = modem.getLocalIP();
    // DBG("IP:", ipAddress);

    String modemInfo = modem.getModemInfo();
    DBG("Modem:", modemInfo);
    




    String ccid = modem.getSimCCID();
    DBG("CCID:", ccid);

    String imei = modem.getIMEI();
    DBG("IMEI:", imei);

    String imsi = modem.getIMSI();
    DBG("IMSI:", imsi);

    // String cop = modem.getOperator();
    // DBG("Operator:", cop);

    SerialMon.print("Attempting to connect to APN: "); SerialMon.println(apn);
    modem.gprsConnect(apn,"","");
    delay(3000);

    if(!modem.isGprsConnected()){
      Serial.println("Couldn't connect");
    }

    nbiotConnected = modem.isGprsConnected();
    DBG("GPRS status:", nbiotConnected ? "connected" : "not connected");

    IPAddress local = modem.localIP();
    DBG("Local IP:", local);

    // nbiotConnected = true;
    delay(3000);
  }
  else if(csq == 99)
  {
    nbiotConnected = false;
    error++;
    DBG("Error:", error);
    delay(1000);
  }



  delay(5000);


}