# Instructions for ArduinoIDE
Make sure to have [ArduinoIDE](https://www.arduino.cc/en/software) installed.

## Setup on Arduino IDE
1. Make sure you have ESP32 boards added to Arduino IDE. If not here is [how](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) to do it. Then make sure to have the following libraries installed: `ArduinoJSON`, `ESPAsyncWebSrv`.

2. Copy the contents inside `ESP32Cam-AWS-MeterReading/ArduinoIDE` folder to new sketch, for creating the header files can create by adding new tab in the top right corner with 3 dots. Instead of adding `secrets_template.h`, make sure to create a `secrets.h` file based on the `secrets_template.h` file and fill in relevant information needed like Wi-fi ssid and password, ESP32-Camera board, and so on.

3. Verify and upload to the ESP32-Camera board, make sure to select correct port and board.

4. Click `Tools` and `Serial Monitor` on the top menu to get device serial outputs.