#include "WiFi.h"
#include "ArduinoJson.h"
#include "base64.h"
#include "esp_camera.h"
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "main.h"
#include "string.h"
// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
// #define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"


// Recheck WiFI every interval
unsigned long wifiCheckPreviousMillis = 0;
unsigned long wifiCheckInterval = 60000;
unsigned long picturePreviousMillis = 0;
unsigned long pictureInterval = 60000*3;


WebServer server(80);


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
    config.pixel_format = PIXFORMAT_JPEG; // for streaming
    // config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.frame_size = FRAMESIZE_HD;

    // Select lower framesize if the camera doesn't support PSRAM
    if(psramFound()){
        config.frame_size = FRAMESIZE_HD; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
        config.jpeg_quality = 10; //10-63 lower number means higher quality
        config.fb_count = 2;
        config.grab_mode = CAMERA_GRAB_LATEST;
    } 
    else {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }



    /* Camera image settings */
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

String takePicture(){
    // capture camera frame
    camera_fb_t *fb = esp_camera_fb_get();
    if(!fb) {
        Serial.println("Camera capture failed");
        return "Error";
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


    // encode buffer to base64
    String encoded = base64::encode(fb->buf, fb->len);

    // Serial.println(encoded);
    // Serial.println("\n\n\n");

    esp_camera_fb_return(fb);
    
    return encoded;
}

void handleRoot() {
    String base64encoded = takePicture();
    // Serial.println(base64encoded);
    String htmlTag = "<img src=\"data:image/jpeg;base64," + base64encoded + "\" style=\"display: block; margin: auto; width: 50%;\">" + "<h4>Raw Base64</h4>" + base64encoded + "</p>";
    // Serial.println(htmlTag);
    if(base64encoded!="Error")
        server.send(200, "text/html", htmlTag);
}

void handleNotFound() {
    String message = "Error 404! File Not Found\n\n";
    server.send(404, "text/plain", message);
}

void startServer(){
    if (MDNS.begin("esp32")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}


void setup(){
    Serial.begin(115200);
    delay(500);
    connectWifi();
    delay(500);
    startServer();
    delay(500);
    cameraConfig();
    delay(500);
}


void loop(){
    server.handleClient();




    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting every interval
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - wifiCheckPreviousMillis >=wifiCheckInterval)) {
        Serial.print(millis());
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        wifiCheckPreviousMillis = currentMillis;
    }

    // take picture interval
    // if(currentMillis - picturePreviousMillis >= pictureInterval){
    //     takePicture();
    //     picturePreviousMillis = currentMillis;
    // }

    delay(2);

}