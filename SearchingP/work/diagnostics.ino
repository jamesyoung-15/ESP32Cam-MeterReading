#include "heltec.h"

#define BAND    433E6
// #define TINY_GSM_MODEM_SIM808
#define TINY_GSM_MODEM_SIM7000


#define SerialMon Serial
#define SerialAT  Serial2

#define TINY_GSM_DEBUG SerialMon
#define DUMP_AT_COMMANDS

#define TINY_GSM_USE_GPRS true

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

  if(modem.getSimStatus() != 3){SerialMon.println("Sim locked");}
    
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
    // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.print("Initializing modem...");
  if (!modem.restart()) {
    // if (!modem.init()) {
    SerialMon.println(F(" [fail]"));
    SerialMon.println(F("************************"));
    SerialMon.println(F(" Is your modem connected properly?"));
    SerialMon.println(F(" Is your serial speed (baud rate) correct?"));
    SerialMon.println(F(" Is your modem powered on?"));
    SerialMon.println(F(" Do you use a good, stable power source?"));
    SerialMon.println(F(" Try using File -> Examples -> TinyGSM -> tools -> AT_Debug to find correct configuration"));
    SerialMon.println(F("************************"));
    delay(10000);
    return;
  }
  SerialMon.println(F(" [OK]"));

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
  // Unlock your SIM card with a PIN if needed
  if (GSM_PIN && modem.getSimStatus() != 3) { modem.simUnlock(GSM_PIN); }
#endif

#if TINY_GSM_USE_WIFI
  // Wifi connection parameters must be set before waiting for the network
  SerialMon.print(F("Setting SSID/password..."));
  if (!modem.networkConnect(wifiSSID, wifiPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
#endif

#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
  // The XBee must run the gprsConnect function BEFORE waiting for network!
  modem.gprsConnect(apn, gprsUser, gprsPass);
#endif

  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork(
          600000L)) {  // You may need lengthen this in poor service areas
    SerialMon.println(F(" [fail]"));
    SerialMon.println(F("************************"));
    SerialMon.println(F(" Is your sim card locked?"));
    SerialMon.println(F(" Do you have a good signal?"));
    SerialMon.println(F(" Is antenna attached?"));
    SerialMon.println(F(" Does the SIM card work with your phone?"));
    SerialMon.println(F("************************"));
    delay(10000);
    return;
  }
  SerialMon.println(F(" [OK]"));

#if TINY_GSM_USE_GPRS
  // GPRS connection parameters are usually set after network registration
  SerialMon.print("Connecting to ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(F(" [fail]"));
    SerialMon.println(F("************************"));
    SerialMon.println(F(" Is GPRS enabled by network provider?"));
    SerialMon.println(F(" Try checking your card balance."));
    SerialMon.println(F("************************"));
    delay(10000);
    return;
  }
  SerialMon.println(F(" [OK]"));
#endif

  IPAddress local = modem.localIP();
  SerialMon.print("Local IP: ");
  SerialMon.println(local);

  SerialMon.print(F("Connecting to "));
  SerialMon.print(server);
  if (!client.connect(server, port)) {
    SerialMon.println(F(" [fail]"));
    delay(10000);
    return;
  }
  SerialMon.println(F(" [OK]"));

//   // Make a HTTP GET request:
//   client.print(String("GET ") + resource + " HTTP/1.0\r\n");
//   client.print(String("Host: ") + server + "\r\n");
//   client.print("Connection: close\r\n\r\n");

//   // Wait for data to arrive
//   while (client.connected() && !client.available()) {
//     delay(100);
//     SerialMon.print('.');
//   };
//   SerialMon.println();

//   // Skip all headers
//   client.find("\r\n\r\n");

//   // Read data
//   uint32_t timeout       = millis();
//   uint32_t bytesReceived = 0;
//   while (client.connected() && millis() - timeout < 10000L) {
//     while (client.available()) {
//       char c = client.read();
//       // SerialMon.print(c);
//       bytesReceived += 1;
//       timeout = millis();
//     }
//   }

//   client.stop();
//   SerialMon.println(F("Server disconnected"));

// #if TINY_GSM_USE_WIFI
//   modem.networkDisconnect();
//   SerialMon.println(F("WiFi disconnected"));
// #endif
// #if TINY_GSM_USE_GPRS
//   modem.gprsDisconnect();
//   SerialMon.println(F("GPRS disconnected"));
// #endif

//   SerialMon.println();
//   SerialMon.println(F("************************"));
//   SerialMon.print(F(" Received: "));
//   SerialMon.print(bytesReceived);
//   SerialMon.println(F(" bytes"));
//   SerialMon.print(F(" Test:     "));
//   SerialMon.println((bytesReceived == 121) ? "PASSED" : "FAILED");
//   SerialMon.println(F("************************"));

  // Do nothing forevermore
  while (true) { delay(1000); }


}




