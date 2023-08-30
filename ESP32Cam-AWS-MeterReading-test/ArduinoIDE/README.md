# Instructions for ArduinoIDE
Demo video here: [Drive Link](https://drive.google.com/drive/folders/1kReh9V65UzdTr_o5L0JZh33rIV5FJ10U?usp=drive_link)
## Setup on Arduino IDE
- Make sure you have ESP32 boards added to Arduino IDE and make sure to have the following libraries installed: ArduinoJSON, ESPAsyncWebServer.

- Copy the contents inside ArduinoSketch folder to new sketch, for creating the header files can create by adding new tab in the top right corner with 3 dots. Instead of adding `secrets_template.h`, make sure to create a `secrets.h` file based on the `secrets_template.h` file and fill in relevant information needed like Wi-fi ssid and password, ESP32-Camera board, and so on.

- Verify and upload to the ESP32-Camera board.

## Usage
- Check the Serial Monitor and if connected to Wi-fi successfully, the device will have an IP address. Go to the IP address in a local browser (make sure it is HTTP not HTTPS).

- The website will show the latest image taken and stored in the ESP32-Camera. To take a new photo, press `take photo` button and to display it on the website, press `check photo` button.

- The image will have a cropping utility. Use the cropping utility to get the region of the photo you want. You can preview what the resulting cropped photo with `preview crop` button.

- Once you are happy with the crop, press `Save Crop Dimension`. This will send the crop information to the ESP32-Camera so that it will forward it to AWS so that AWS can do the image cropping there.

- Then, if you want to send to AWS, press `Send AWS`. Alternatively, the code also has a section to send photos periodically to AWS in the `void loop` part.

## Important Notes
Remember the website only shows the latest STORED photo, so what the website displays may be different to what the camera actually sees. To update it just press the `take picture` and `check photo` buttons.

Please keep the camera as still as possible. Also, if you cropped the photo and sent the crop dimensions to ESP32-Cam but then changed the camera position later, you most likely need to update the crop dimensions by repeating the process of taking a photo, reloading the page, preview cropping, and sending crop dimensions to update it.