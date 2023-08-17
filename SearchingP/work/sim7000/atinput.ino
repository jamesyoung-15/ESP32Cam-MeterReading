/**************************************************************
 *
 * This script tries to auto-detect the baud rate
 * and allows direct AT commands access
 *
 * TinyGSM Getting Started guide:
 *   https://tiny.cc/tinygsm-readme
 *
 **************************************************************/

// Select your modem:
// #define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM7000
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM868
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_M95
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_MC60
// #define TINY_GSM_MODEM_MC60E
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial
#define SerialAT  Serial2

#include "heltec.h"
#include <TinyGsmClient.h>

#define BAND    433E6

#define TINY_GSM_DEBUG SerialMon

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

// Module baud rate
uint32_t rate = 0; // Set to 0 for Auto-Detect

void setup() {

  // init heltec device
  Heltec.begin(false /*DisplayEnable*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND /*long BAND*/);
  // init serial ports
  SerialMon.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, 17,13);
  
  SerialMon.println("Restarting Module...");
  modem.restart();
  
  delay(200);
  SerialMon.println("Init Done. Can enter AT commands. Choose Both NL and CR option.");
  // modem.factoryDefault();
}

void loop() {
  while(true) {
    // read from sim module
    if (SerialAT.available()) {
      SerialMon.write(SerialAT.read());
    }
    // read from serial input
    if (SerialMon.available()) {
      SerialAT.write(SerialMon.read());
    }
    delay(1);
  }
}