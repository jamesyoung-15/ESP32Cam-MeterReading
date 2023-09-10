# Code Documentation
This page will explain the code and how to develop and improve the project. Please note the code is messy and not great as I am an inexperienced intern, so if the progress so far isn't good enough feel free to completely scrap it.

## Lambda Functions
Run-down of Lambda functions and how to improve/modify them.
### Upload to S3 Lambda
This Lambda function in [here](../AWS/lambda_functions/uploadToS3.js) accepts the image, crop, and rotation data from API gateway and uses Node module Sharp to crop and rotate the image based on the passed data. This code works but it assumes the user does not send weird cropping data (ie. rotate by 90 degrees and then included regions out of the image in the CropperJS) and may throw error if user does so. To mitigate this, do some checks (ie. check crop dimensions vs. image dimensions).


### S3 Trigger Rekognition
The main issue with [this](../AWS/lambda_functions/s3TriggerRekognition.py) function is that there is no image processing after cropping the image. Ideally, if you have experience w/ image processing and recognition, most likely the image needs to be filtered and worked with. Also with proper knowledge of machine learning there is no need to use AWS Rekognition as it is not designed for this use case and is expensive. Instead, train your own model (using water meters or whatever digital utility) as it will be more accurate to utility meters rather than rely on OCR readers. Something like a Tensorflow model for reading digital meters like [this](https://medium.com/analytics-vidhya/digit-recognition-of-digital-meters-using-tensorflow-2-object-detection-api-48364cd678a9) is much better than using Rekognition (also saves cost) as it is taylored for meters since some meters may not have clear digits.

### Restless API with API Gateway, Lambda, and DynamoDB
Not much to discuss for [this](../AWS/lambda_functions/db_api.js), plenty of resources online about how to [build this API](https://www.youtube.com/watch?v=Ut5CkSz6NR0&t=271s) and then if you want to display it on a website simply look up some tutorial of how to integrate an API to a front-end (basically just look up some tutorial on how to display weather data from some weather API, same logic).

## ESP32-CAM Code

### Website
Use the [index.html](../ESP32Cam-AWS-MeterReading/index.html) file for making modifications to the website, where you can change the UI/UX, the Javascript, and so on. Once you want to test the changes on the ESP32-CAM, copy and paste the `index.html` file into the [html.h](../ESP32Cam-AWS-MeterReading/src/html.h) (make sure it's between the `R"rawliteral(` to `)rawliteral";`). Then you have to upload the code to the ESP32-CAM to test it.

In [main.cpp](../ESP32Cam-AWS-MeterReading/src/main.cpp), you can see some examples on how to implement certain features. In `void beginServer()` function, it shows you how to use the webserver to access and send data to and from a web browser to the ESP32-CAM. For example to get the crop dimensions and save it to ESP32-CAM this part of the code:
``` C++
server.on("/cropData", HTTP_GET, [](AsyncWebServerRequest * request) {
        String left = request->getParam("cropLeft")->value();
        String top = request->getParam("cropTop")->value();
        String width = request->getParam("cropWidth")->value();
        String height = request->getParam("cropHeight")->value();
        String rotate = request->getParam("rotate")->value();
        cropLeft = left.toInt();
        cropTop = top.toInt();
        cropWidth = width.toInt();
        cropHeight = height.toInt();
        rotateAngle = rotate.toInt();
        request->send_P(200, "text/plain", "Received crop data");
        checkCropDim();
    });
```
shows how the server listens to a specific url path with `/cropData`, and then everything afterwards in the URL is the crop and rotation data (eg. `/cropData?cropLeft=${cropLeft}&cropTop=${cropTop}&cropWidth=${cropWidth}&cropHeight=${cropHeight}&rotate=${rotateValue}` in index.html code). 

For sending data to web browser this code below shows sending an image on a get request with that URL path. You can also send text data for maybe displaying some other info like camera configuration, send status, and so on.
``` C++
server.on("/saved-photo.jpg", HTTP_GET, [](AsyncWebServerRequest * request) {
        Serial.println("Sending photo");
        request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
});
```



There are many modifications and improvements here as well to be made: 
- You may want to add more options to configure the camera (ie. change photo quality, lighting, and so on) similar to how the Arduino example for ESP32-CAM server does it. You might do this by creating a form and allowing users to input some variables like photo quality, then send the data to the ESP32-CAM. The ESP32-CAM can then read the variables and reconfigure the camera.
- You may also want to add option to allow user to input the send interval time rather than hard code it.
- Improve the cropping function as when it rotates the image it shows out of boundary parts.
- Also add better confirmation parts when sending commands, most importantly when we send the image to AWS there currently is no check for whether AWS has actually received the image properly. To do this we can may add a variable to indicate last sent AWS image status, set it to true when the status code 200 is received in the `void sendPhotoToS3()` function in [`main.cpp`](../ESP32Cam-AWS-MeterReading/src/main.cpp) (or `sketch.ino` if using ArduinoIDE), and then the website can query for this variable to check whether the most recent sent image to AWS was sucessful or not.




