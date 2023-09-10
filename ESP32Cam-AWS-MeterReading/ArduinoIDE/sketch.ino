/* Arduino Libraries */
#include "ArduinoJson.h"
#include "base64.h"
#include "esp_camera.h"
#include <WiFiClient.h>
#include "SPIFFS.h"
#include "ESPAsyncWebSrv.h"
#include "WiFi.h"
#include <ESPmDNS.h>
#include "string.h"
#include <HTTPClient.h>

/* Own Includes */
#include "secrets.h"
#include "camera_pins.h"
#include "html.h"

/* Global Variables */

// Recheck WiFI every interval
// unsigned long wifiCheckPreviousMillis = 0;
// unsigned long wifiCheckInterval = 60000;

// Take Photo every interval
unsigned long picturePreviousMillis = 0;
unsigned long pictureInterval = 60000;

// Alert variables
boolean takeNewPhoto = false;
boolean sendToAWS = false;
boolean sendToAWSEveryInterval = false;

// Instantiate class variables
AsyncWebServer server(80);
WiFiClientSecure client;

unsigned short cropLeft = 0; 
unsigned short cropTop = 0;
unsigned short cropWidth = 1600;
unsigned short cropHeight = 1200;
short rotateAngle = 0;

// For using external RAM with ArduinoJSON
// struct SpiRamAllocator {
//   void* allocate(size_t size) {
//     return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
//   }

//   void deallocate(void* pointer) {
//     heap_caps_free(pointer);
//   }

//   void* reallocate(void* ptr, size_t new_size) {
//     return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
//   }
// };
// using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;

void connectWifi(){
    // set to station mode
    WiFi.mode(WIFI_STA);
    // disconnect any previous connection
    WiFi.disconnect();

    // begin wifi, connect to wifi with ssid and password defined
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    Serial.print("Connecting to WiFi with SSID of: "); Serial.println(WIFI_SSID);
    Serial.print("Connecting ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    Serial.println("Success!");
    Serial.print("IP Address: "); Serial.println(WiFi.localIP());
}

void scanWifi(){
    Serial.println("Wifi scan... ");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("Scan done!");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        delay(10);
        }
    }
}

void cameraConfig() {
    camera_config_t config;
    /* Hardware Setup */
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 20000000;
    /* Camera Settings */
    config.pixel_format = PIXFORMAT_JPEG; // good for most use cases 
    // config.pixel_format = PIXFORMAT_RGB565; // if cropping, use this
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.frame_size = FRAMESIZE_UXGA;
    config.grab_mode = CAMERA_GRAB_LATEST;
    // config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    // Select lower framesize if the camera doesn't support PSRAM
    // if(psramFound()){
    //     config.frame_size = FRAMESIZE_HD; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    //     config.jpeg_quality = 10; //10-63 lower number means higher quality
    //     config.fb_count = 2;
    //     config.grab_mode = CAMERA_GRAB_LATEST;
    // } 
    // else {
    //     config.frame_size = FRAMESIZE_VGA;
    //     config.jpeg_quality = 12;
    //     config.fb_count = 1;
    // }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    // /* Camera image settings */
    sensor_t * s = esp_camera_sensor_get();

    s->set_brightness(s, 1);     // -2 to 2
    s->set_contrast(s, 0);       // -2 to 2
    s->set_saturation(s, 0);     // -2 to 2
    s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
    s->set_aec2(s, 0);           // 0 = disable , 1 = enable
    s->set_ae_level(s, 0);       // -2 to 2
    s->set_aec_value(s, 300);    // 0 to 1200
    s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0);       // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    s->set_bpc(s, 0);            // 0 = disable , 1 = enable
    s->set_wpc(s, 1);            // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
    s->set_lenc(s, 1);           // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
    s->set_vflip(s, 0);          // 0 = disable , 1 = enable
    s->set_dcw(s, 1);            // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);       // 0 = disable , 1 = enable

}

void sendPhotoToS3(){
    // check wifi
    if(WiFi.status() != WL_CONNECTED){
            Serial.println("Cannot send: Not connected to Wifi!");
            return;
    }

    else{
        // capture camera frame
        camera_fb_t *fb = esp_camera_fb_get();
        if(!fb) {
            Serial.println("Camera capture failed");
            return;
        } else {
            Serial.println("Camera capture successful!");
        }

        const char *data = (const char *)fb->buf;
        // Image metadata
        Serial.print("Size of image:");
        Serial.println(fb->len);
        Serial.print("Shape->width:");
        Serial.print(fb->width);
        Serial.print("height:");
        Serial.println(fb->height);
        Serial.println("\n\n");
    
        HTTPClient http;

        Serial.println("Sending to AWS");
        // take and get encoded image, get formatted date to use as filename, package it as json, send to aws api
        String url = String(AWS_REST_API) + "/" + String(cropLeft) + "/" + String(cropTop) + "/" + String(cropWidth) + "/" + String(cropHeight) + "/" + String(rotateAngle);
        http.begin(client, url);
        Serial.println(url);
        http.addHeader("Content-Type", "image/jpg");
        // http.addHeader("Content-Encoding", "base64");
                
        
        // Serial.println(dataToSend);
        int httpResponseCode = http.POST(fb->buf, fb->len);
        if(httpResponseCode>0){

            String response = http.getString();  //Get the response to the request
        
            Serial.print("Response code: "); Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);           //Print request answer
        }
        else{
            Serial.print("Error on sending POST: ");
            Serial.print("Response code: "); Serial.println(httpResponseCode); 
        }
        esp_camera_fb_return(fb);
    }
}

void checkCropDim(){
    Serial.println("Received crop data of: ");
    Serial.print("X: "); Serial.println(cropLeft);
    Serial.print("Y: "); Serial.println(cropTop);
    Serial.print("Width: "); Serial.println(cropWidth);
    Serial.print("Height: "); Serial.println(cropHeight); 
    Serial.print("Rotate Degrees: "); Serial.println(rotateAngle);
}

void beginServer(){
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send_P(200, "text/html", index_html);
    });

    server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send_P(200, "text/plain", "Taking Photo");
        takeNewPhoto = true;
    });

    server.on("/startInterval", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send_P(200, "text/plain", "Sending to AWS every interval seconds");
        sendToAWSEveryInterval = true;
        Serial.print("Will start sending pictures to AWS every "); Serial.print(pictureInterval); Serial.println(" millisec");
    });

    server.on("/stopInterval", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send_P(200, "text/plain", "Stopping AWS interval send");
        sendToAWSEveryInterval = false;
        Serial.println("Stop sending to AWS in intervals");
    });

    server.on("/saved-photo.jpg", HTTP_GET, [](AsyncWebServerRequest * request) {
        Serial.println("Sending photo");
        request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
    });

    server.on("/send-aws", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send_P(200, "text/plain", "Sending to AWS");
        sendToAWS = true;
        Serial.println("Start test sending to AWS");
    });

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


    // Start server
    server.begin();
}

// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

// Capture Photo and Save it to SPIFFS
void capturePhotoSaveSpiffs( void ) {
  camera_fb_t * fb = NULL; // pointer
  bool ok = 0; // Boolean indicating if the picture has been taken correctly

  do {
    // Take a photo with the camera
    Serial.println("Taking a photo...");

    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }


    // Photo file name
    Serial.printf("Picture file name: %s\n", FILE_PHOTO);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);

    // Insert the data in the photo file
    if (!file) {
      Serial.println("Failed to open file in writing mode");
    }
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    // Close the file
    file.close();
    esp_camera_fb_return(fb);

    // check if file has been correctly saved in SPIFFS
    ok = checkPhoto(SPIFFS);
  } while ( !ok );
}



void setup(){
    Serial.begin(115200);
    Serial.println("Configuring...");
    delay(500);
    // For init internal ESP32 file system
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        ESP.restart();
    }
    else {
        delay(500);
        Serial.println("SPIFFS mounted successfully");
    }
    cameraConfig();
    delay(500);
    capturePhotoSaveSpiffs();
    delay(2000);
    // scanWifi();
    // delay(500);
    connectWifi();
    delay(500);
    delay(100);
    beginServer();
    delay(100);
    client.setInsecure(); // NOT RECOMMENDED, use a proper cert in production
    Serial.println("Ready to use!\n");
}


void loop(){
    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting every interval
    // take picture interval
    if(currentMillis - picturePreviousMillis >= pictureInterval && sendToAWSEveryInterval){
        // check wifi
        if(WiFi.status() != WL_CONNECTED){
            Serial.print(millis());
            Serial.println("Reconnecting to WiFi...");
            WiFi.disconnect();
            WiFi.reconnect();
        }

        else{
            sendPhotoToS3();
            sendToAWS = false;
        }
        picturePreviousMillis = currentMillis;

    }

    if (takeNewPhoto) {
        capturePhotoSaveSpiffs();
        takeNewPhoto = false;
    }
    if (sendToAWS) {
        sendPhotoToS3();
        sendToAWS = false;
    }

    delay(2);
}