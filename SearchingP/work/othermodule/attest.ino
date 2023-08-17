#define SerialAT Serial2
#define SerialMon Serial

// Assuming ESP32 Wroom
#define RXD2 16
#define TXD2 17

void setup() {
    // put your setup code here, to run once:
    SerialMon.begin(115200);
    SerialAT.begin(9600, SERIAL_8N1, RXD2, TXD2);  
    delay(1000);

    SerialMon.println("Init done");

  
}

void loop() {
    // put your main code here, to run repeatedly:
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
