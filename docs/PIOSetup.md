# PlatformIO Project Instructions
1. Make sure to have either PlatformIO or PlatformIO CLI installed. Installation and setup of PlatformIO for VSCode is [here](https://platformio.org/install/ide?install=vscode). 

1. Clone this repository. If not comfortable with CLI, then just press [this](https://github.com/jamesyoung-15/kolour-think-tank-internship/archive/refs/head/main.zip) to repository download as ZIP.
``` bash
git clone https://github.com/jamesyoung-15/kolour-think-tank-internship
```

#### With PlatformIO CLI
2. Enter into ESP32Cam-AWS-MeterReading directory of this repository.
``` bash
cd kolour-think-tank-internship/ESP32Cam-AWS-Meter-Reading
```

3. Create a `secrets.h` file. Copy and paste the contents of `secrets_template.h` into the `secrets.h` file, then edit the relevant information.

4. Build this project, upload it to the USB port with ESP32-CAM connected, and monitor the device.
``` bash
pio run --target upload --upload-port /dev/ttyUSB0 && pio device monitor
```


#### With GUI (eg. VSCode)
2. Open the `kolour-think-tank-internship/ESP32Cam-AWS-Meter-Reading` folder with VSCode or whatever IDE you're using with PlatformIO.

3. Click the PlatformIO icon on the left activity bar. Press `build` and then `upload and monitor`.