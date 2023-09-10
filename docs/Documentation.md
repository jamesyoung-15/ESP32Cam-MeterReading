# Documentation for ESP32-Camera Meter Reading Project
Documentation and usage guide for ESP32-Camera Meter Reading Project. For video on how to setup click [video guide](https://youtu.be/pHNRqF-2EDg).

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


### Phyiscal Setup
Make sure the ESP32-CAM is pointed to the digital part of the utility meter. Try to get it as close as possible (~4cm). Keep the camera stable and make sure it won't. Try to get good lighting while also avoiding reflection if possible.

### Website Config
After uploading the code with whichever framework, the Serial Monitor will display the IP address. Go to that IP address in a web browser (eg. `http://192.168.1.29`). Use the website to configure the image crop then start the send to AWS when the settings are fine. 

Click here for for more [detailed instructions and demo](./WebsiteConfig.md).

## Improvements/Code Explanation
Not fully complete, but [here](./CodeGuide.md).

## Contact
Any problems feel free to contact me.

## Resources Used
- Lambda Layer:
    - [Sharp](https://sharp.pixelplumbing.com/) (packaged layer [here](https://github.com/Umkus/lambda-layer-sharp))
    - [Python Pillow](https://pypi.org/project/Pillow/) (packaged layer [here](https://github.com/keithrozario/Klayers/tree/master/deployments/python3.8))

- Documentation and Articles Used
    - [AWS Documentation](https://docs.aws.amazon.com/index.html)
    - Useful Rekognition Material:
        - [Text Detection Rekognition](https://docs.aws.amazon.com/rekognition/latest/dg/text-detecting-text-procedure.html)
        - [Analyze image from S3](https://docs.aws.amazon.com/rekognition/latest/dg/images-s3.html)
        - [Draw boundary boxes](https://docs.aws.amazon.com/rekognition/latest/dg/images-displaying-bounding-boxes.html)
        - [Trigger Rekognition from S3](https://docs.aws.amazon.com/rekognition/latest/dg/images-lambda-s3-tutorial.html)
        - [Store Rekogniton result to DynamoDB](https://docs.aws.amazon.com/rekognition/latest/dg/storage-tutorial.html)
    - Creating API for DynamoDB Material:
        - [Building Serverless API w/ DynamoDB, API Gateway, Lambda](https://www.youtube.com/watch?v=Ut5CkSz6NR0&t=271s)
    - Creating API for Accepting Images:
        - [ESP32 Cam To AWS S3 Upload](https://www.youtube.com/watch?v=FIPkU-gx_kU&t=227s)
        - [Sharp Documentation](https://sharp.pixelplumbing.com/)
    
