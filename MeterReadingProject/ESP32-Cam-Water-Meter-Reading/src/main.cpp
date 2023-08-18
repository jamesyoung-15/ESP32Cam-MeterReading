#include "WiFi.h"
#include "ArduinoJson.h"
#include "base64.h"

const char* WIFI_SSID = "VIP_24G";
const char* WIFI_PASSWD = "1357908642";

void connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    Serial.print("Connecting to WiFi with SSID of: "); Serial.println(WIFI_SSID);
    Serial.print("Connecting ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    Serial.println("Success!");
    Serial.print("IP Address: "); Serial.println(WiFi.localIP());
}


void setup(){
    Serial.begin(115200);

    connectWifi();

}


void loop(){
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        delay(10);
        }
    }
    Serial.println("");

    // Wait a bit before scanning again
    delay(5000);

}