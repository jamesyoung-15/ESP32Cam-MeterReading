# Water Meter Reading w/ ESP32-Camera
Project for getting water meter reading using ESP32-Camera.
## Brainstorm
2 main approaches:

First approach is edge AI approach, where the approach uses something like Edge Impulse to train and create a TinyML model with water meter data and then deploy it on an ESP32-Camera module.

Second approach is using another machine to to process and extract the meter reading from the image. This can be done with Cloud-based AI platform like Google's Cloud Vision API or a local machine. The ESP32-Camera sends the image data (in base64 encoded) to machine/server.