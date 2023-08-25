# Water Meter Reading w/ ESP32-Camera
This on-going project attempts to use image recognition to obtain water meter reading from images taken from an ESP32-Camera.

## Documentation
### Project Details
- Hardware: ESP32-Camera
- Framework: PlatformIO
- Libraries: Arduino-ESP32

### Setup (tested on Linux)
- Clone this repo and enter this directory
``` bash
git clone 
cd ESP32-Cam-Water-Meter-Reading
```

- Add your config settings by creating a new `secrets.h` file in the `lib/Config folder`. Copy and paste the `secrets_example.h` and then modify the variables to add settings such as Wi-fi SSID and password, AWS credentials, and so on. 

- Compile/build the project with either PlatformIO CLI or use the graphical interface to build (make sure the upload port is the correct port to the ESP32-Camera)
``` bash
pio run --target upload --upload-port /dev/ttyUSB0
```
- You can use a serial monitor program to check serial logs or run `pio device monitor` w/ PlatformIO CLI.
