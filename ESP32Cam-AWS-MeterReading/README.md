# Water Meter Reading w/ ESP32-Camera
This on-going project attempts to use image recognition to obtain water meter reading from images taken from an ESP32-Camera.

## Documentation
### Project Details
- Hardware: ESP32-Camera
- Frameworks: PlatformIO, Arduino IDE
- Additional Libraries: Arduino-ESP32, ArduinoJSON, ESPAsyncWebServer


### Setup on Arduino IDE
- Make sure you have ESP32 boards added to Arduino IDE and make sure to have the following libraries installed: ArduinoJSON, ESPAsyncWebServer.

- Copy the contents inside ArduinoSketch folder to new sketch, for creating the header files can create by adding new tab in the top right corner with 3 dots. Make sure to create a `secrets.h` file based on the `secrets_template.h` file and fill in relevant information needed like Wi-fi ssid and password, ESP32-Camera board, and so on.

- Verify and upload to the ESP32-Camera board.

### Platformio Setup (tested on Linux)
- Add your config settings by creating a new `secrets.h` file in the `src`. Copy and paste the `secrets_example.h` and then modify the variables to add settings such as Wi-fi SSID and password, AWS credentials, and so on. 

- Compile/build the project with either PlatformIO CLI or use the graphical interface to build (make sure the upload port is the correct port to the ESP32-Camera)
``` bash
pio run --target upload --upload-port /dev/ttyUSB0
```
- You can use a serial monitor program to check serial logs or run `pio device monitor` w/ PlatformIO CLI.

