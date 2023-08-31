# Documentation for ESP32-Camera Meter Reading Project
Documentation and usage guide for ESP32-Camera Meter Reading Project.

## Requirements
### Hardware
- ESP32-CAM module 
- Way to flash code to ESP32-CAM
    - eg: ESP32-CAM-MB, USB to TTL, etc. 

No need for SD card as this project uses the onboard SPIFFS filesystem for storing recent camera pictures to display to web.

### Software
- PlatformIO or Arduino IDE
- Arduino Libraries: Arduino-ESP32, ArduinoJSON, ESPAsyncWebServer
- AWS (can use free-tier otherwise will be billed b/c of Rekognition)
    - Services Used: Rekognition, Lambda, API Gateway, S3, DynamoDB

## AWS Setup
Click [here](./AWS_Setup.md) for instructions.


## ESP32-Camera Setup
This project has been tested with ArduinoIDE and PlatformIO. It mainly uses Arduino-ESP32 and some other Arduino Libraries due to easy use.
### PlatformIO
Click [here](./PIOSetup.md) for instructions.

### Arduino IDE
Click [here](./ArduinoSetup.md) for instructions.


### Website Config
- After uploading the code with whichever framework, the Serial Monitor will display the IP address of the device if successfully connected to Wi-Fi. Go to that IP address in a web browser (eg. `http://192.168.1.29`). Make sure it is HTTP instead of HTTPS. Also make sure you are connected to the same network as the ESP32-CAM.

- The website will show the latest image taken and stored in the ESP32-Camera. To take a new photo, press `take photo` button and to display it on the website, press `check photo` button.

- The image will have a cropping utility. Use the cropping utility to get the region of the photo you want. You can preview what the resulting cropped photo with `preview crop` button.

- Once you are happy with the crop, press `Save Crop Dimension`. This will send the crop information to the ESP32-Camera so that it will forward it to AWS so that AWS can do the image cropping there.

- Then, if you want to send to AWS, press `Send AWS`. Alternatively, the code also has a section to send photos periodically to AWS in the `void loop` part.

## Important Notes
- Remember the website only shows the latest STORED photo, so what the website displays may be different to what the camera actually sees if you moved it. To update it just press the `Take New Picture`.

- Please keep the camera as still as possible. Also, if you cropped the photo and sent the crop dimensions to ESP32-Cam but then changed the camera position later, you most likely need to update the crop dimensions by repeating the process of taking a photo, preview cropping, and sending crop dimensions to update it.


### Phyiscal Setup
Make sure the ESP32-CAM is pointed to the digital part of the utility meter. Try to get it as close as possible (~4cm). Keep the camera stable and make sure it won't. Try to get good lighting while also avoiding reflection if possible.