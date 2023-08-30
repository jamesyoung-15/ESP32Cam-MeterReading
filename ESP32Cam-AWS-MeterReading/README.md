# Water Meter Reading w/ ESP32-Camera
This on-going project attempts to use image recognition to obtain water meter reading from images taken from an ESP32-Camera.

## Demo

API Link to View DynamoDB Entries: [Link](https://i8ave7ovua.execute-api.us-east-1.amazonaws.com/testv1/watermeter/alldata)

## Documentation
### Project Details
- Hardware: ESP32-Camera
- Frameworks: PlatformIO, Arduino IDE
- Additional Libraries: Arduino-ESP32, ArduinoJSON, ESPAsyncWebServer


### Setup on Arduino IDE
Files and instructions are in [here](./ArduinoIDE/).

### Platformio Setup (tested on Linux)
- Prequisites: Make sure to either have PlatformIO CLI or PlatformIO IDE or VSCode PlatformIO plugin.

- Add your config settings by creating a new `secrets.h` file in the `src`. Copy and paste the `secrets_example.h` and then modify the variables to add settings such as Wi-fi SSID and password, AWS info, and so on. 

- Compile/build the project with either PlatformIO CLI or use the graphical interface to build (make sure the upload port is the correct port to the ESP32-Camera)
``` bash
pio run --target upload --upload-port /dev/ttyUSB0
```
- You can use a serial monitor program to check serial logs or run `pio device monitor` w/ PlatformIO CLI.

