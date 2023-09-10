# Website Config

## Instructions
- After uploading the code with whichever framework, the Serial Monitor will display the IP address of the device if successfully connected to Wi-Fi. Go to that IP address in a web browser (eg. `http://192.168.1.29`). Make sure it is HTTP instead of HTTPS. Also make sure you are connected to the same network as the ESP32-CAM.

The website should look something like this:
![](../assets/images/website-example.png)

- The website will show the latest image taken and stored in the ESP32-Camera. To take a new photo, press `Take New Photo` button.

- The image will have a cropping and rotation utility. Use the cropping utility and rotation slider to get the region of the photo you want. You can preview what the resulting cropped photo with `preview crop` button.

- Once you are happy with the crop, press `Save Crop Dimension`. This will send the crop information to the ESP32-Camera so that it will forward it to AWS so that AWS can do the image cropping there. Ideally this should just be configured once the camera is completely still and gets a good angle of the meter so that the crop dimension only has to be saved once and then every image sent will have that crop result.

- Then, to test send to AWS, press `Test Send AWS`. If everything is working fine, then you can press the `Send to AWS in Intervals` button where the ESP32-CAM will send a picture every few minutes (can be changed in code). The results should be able to be seen in the S3 bucket and API access for the DynamoDB table.

## Important Notes
- Remember the website only shows the latest STORED photo, so what the website displays may be different to what the camera actually sees if you moved it. To update it just press the `Take New Photo`.

- Please keep the camera as still as possible. Also, if you cropped the photo and sent the crop dimensions to ESP32-Cam but then changed the camera position later, you most likely need to update the crop dimensions by repeating the process of taking a photo, preview cropping, and sending crop dimensions to update it. If you haven't moved the camera and the previous crop worked then you don't need to update it everytime a picture is sent.