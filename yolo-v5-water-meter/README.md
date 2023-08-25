# YoloV5 Water Meter Custom Model
The water meter image recognition uses YoloV5 (computer vision model). Model was trained with dataset found online and model weights exported as the file `watermeter.pt`. ESP32-Camera uses the Arduino ESP32-Camera web server code example. Using dataset found online, I trained a quick YoloV5 model for testing/demo.

## Documentation
For ESP32-Camera, use the example ESP32-Camera web server sketch from Arduino-ESP32.

## Usage on Windows
### Setting up Yolo V5
1. Make sure to you have Python and Pip.
2. Download yolov5 files [link](https://github.com/ultralytics/yolov5).
3. Open command prompt in that folder, then enter `pip install -r requirements.txt `.

### Demo Instructions
Assuming everything setup (YoloV5 downloaded, weights imported), proceed below:
1. Go to File Explorer.
2. Go to the YoloV5 folder (should be in Downloads).
3. Open the command prompt by selecting the entire text bar (should say something like This PC -> Desktop -> ...) and then replace it with the word `cmd` and press enter. See below for clarity:  

![](./windowsexample.png)

4. In the command prompt, enter `python ./detect.py --weights watermeter.pt --hide-conf --source "http://<ReplaceWithIP>:81/stream"`. This should open up the video stream and show the model image recognition part. This may take a few seconds so just wait. Replace `<ReplaceWithIP>` part with the ESP32-Camera IP which will be something like `192.168.1.12`.
5. To exit/kill the program, pressing X on the video stream may not end it. You need to go back to command prompt and then press `ctrl c` or force quit the command prompt to kill the process.

## Camera Settings
If you wish to change the stream quality or other camera settings in general, enter the ip of the ESP32-Camera in your web browser (eg. http://192.168.1.12) and change the settings there. It will look something like this:

![](./webconfigexample.png)

Note: Higher quality may help with image recognition but will be a lot slower frames.



## Images used for testing
I put a few of the images that seemed to do okay with the model here in this folder.

