# ESP32-Camera Web Server
Connects to Wi-fi and creates an HTTP camera stream viewable on a browser. Taken from Arduino example. 

## Setup
Create a secrets.h file in the `src/` folder and copy and paste the contents of `secretes_example.h`. Change the `const char* ssid = "******"` and `const char* password = "******"`.

## Usage
Use Platform IO in any way you want (VSCode plugin, other IDE, CLI, etc.). Tested with Platform IO CLI (change to relevant port if needed):

```bash 
pio run --target upload --upload-port /dev/ttyUSB0
```

```bash
pio device monitor -p /dev/ttyUSB0
```