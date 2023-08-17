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




TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient  mqtt(client);

void setup() {
  // put your setup code here, to run once:
  Heltec.begin(true /*DisplayEnable*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND /*long BAND*/);
  SerialAT.begin(9600, SERIAL_8N1, 17,13);
  delay(200);

  modem.restart();
  delay(1000);

  // check sim
  sendATCommand("AT+CPIN?");
  delay(50);

  // set mode, 2 Automatic 13 GSM only 38 LTE only 51 GSM and LTE only
  SerialMon.println("Set to LTE mode");
  sendATCommand("AT+CNMP=38");
  delay(50);

  // choose between CAT-M and NB-IoT, 1 cat-m, 2 nb-iot, 3 both
  SerialMon.println("Set to NB-IoT");
  sendATCommand("AT+CMNB=2");
  delay(50);

  // Config NB-IOT Scrambling Feature, 1 for on
  SerialMon.println("Set to LTE mode");
  sendATCommand("AT+NBSC=1");
  delay(50);

  // Check the state of GPRS attachment, should return like 1
  sendATCommand("AT+CGATT?");
  delay(500);

  // gives information about the registration status and access technology of the serving cell.
  sendATCommand("AT+CREG?");

  
  // Inquiring UE system information
  sendATCommand("AT+CPSI?");
  delay(50);
  

  // check current mode and the currently selected operator, should return like + cops: 0, 0,0," CHINA MOBILE",7
  sendATCommand("AT+COPS?");
  delay(50);

  // Get network APN in CAT-M or NB-IOT
  // should return +CGNAPN: <valid>,<Network_APN>, <valid> = 0 means network did not sent APN parameter
  sendATCommand("AT+CGNAPN");


  // Select the Mode of Application Configure APN, 1 is manual (needed for gprs)
  sendATCommand("AT+CAPNMODE=1");
  delay(50);

  // Shut PDP context, https://stackoverflow.com/questions/37385412/getting-error-on-atcstt-and-atcifsr
  sendATCommand("AT+CIPSHUT");
  delay(500);

  // Connect to APN
  SerialMon.println("Attempt connect to APN");
  sendATCommand("AT+CSTT=\"mobile.lte.three.com.hk\"");

  delay(3000);

  // sendATCommand("AT+CIPSTART");

  // Bring Up Wireless Connection with GPRS, should return like ok
  sendATCommand("AT+CIICR");
  delay(500);

  // Get local ip
  sendATCommand("AT+CIFSR");
  delay(50);
  

  

  delay(5000);

    
}

int error = 0;

void loop() {
  // restart module if problem with measuring signal quality
  if(error>3){
    SerialMon.println("Restarting module");
    modem.restart();
    delay(1000);
    error = 0;
  }
  
  // check signal
  int csq = modem.getSignalQuality();
  SerialMon.print("Signal Quality: "); SerialMon.println(csq);
  // check network
  bool connectionCheck = modem.isNetworkConnected();
  SerialMon.print("Connection: "); SerialMon.println(connectionCheck);
  // attempt reconnection
  if(csq!=99 && connectionCheck == 0){
    SerialMon.println("Attempt Reconnect");
    modem.gprsConnect(apn);
    delay(10000);
  }
  else if(csq==99){
    error++;
  }
  

  // ping google
  // sendATCommand("AT+CIPPING=\"www.google.com\"");
  // delay(50);


  

  delay(3000);

}

void sendATCommand(String command){
  SerialAT.println(command);
  while(SerialAT.available()){
    SerialMon.println(SerialAT.readString());
  }
}



