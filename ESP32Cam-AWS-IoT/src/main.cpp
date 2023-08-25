
/* Arduino Libraries */
#include "ArduinoJson.h"
#include "base64.h"
#include "string.h"
#include "esp_camera.h"
#include <WiFiClient.h>
#include <WebServer.h>
#include "WiFi.h"
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include "PubSubClient.h"

// ===================
// Select camera model
// ===================
#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
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

#include "camera_pins.h"
#include "secrets.h"



// Recheck WiFI every interval
// unsigned long wifiCheckPreviousMillis = 0;
// unsigned long wifiCheckInterval = 60000;
unsigned long picturePreviousMillis = 0;
unsigned long pictureInterval = 60000*1;



WebServer server(80);
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

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

void messageHandler(char* topic, byte* payload, unsigned int length){
    Serial.print("incoming: ");
    Serial.println(topic);

    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    const char* message = doc["message"];
    Serial.println(message);
}

void connectAWS()
{
    // Configure WiFiClientSecure to use the AWS IoT device credentials
    net.setCACert(AWS_IOT_CERT_CA);
    net.setCertificate(AWS_IOT_DEVICE_CERT);
    net.setPrivateKey(AWS_IOT_PRIVATE_KEY);

    // Connect to the MQTT broker on the AWS endpoint we defined earlier
    client.setServer(AWS_IOT_ENDPOINT, 8883);

    client.setBufferSize(40000);

    // Create a message handler
    client.setCallback(messageHandler);


    Serial.println("Connecting to AWS IOT");

    while (!client.connect(AWS_IOT_THINGNAME))
    {
        Serial.print(".");
        delay(100);
    }

    if (!client.connected())
    {
        Serial.println("AWS IoT Timeout!");
        return;
    }

    // Subscribe to a topic
    //   client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

    Serial.println("AWS IoT Connected!");
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
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.frame_size = FRAMESIZE_VGA;
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

/* Takes picture with camera and returns base64 encoded image */
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





void publishMessage(){
    String encodedImage = takePicture();
    String folderName = "TestDevice";
    String dataToSend = "{\"S3Folder\": \"" + folderName + "\",  \"base64Image\": \"" + encodedImage +"\"}";
    const char* cString = dataToSend.c_str();

    Serial.println(cString);
    // Serial.println(strlen(cString));

    bool result = client.publish(AWS_IOT_PUBLISH_TOPIC, cString);
    Serial.println((result) ? "Message sent to AWS!" : "Message failed to send to AWS");
}


void setup(){
    Serial.begin(115200);
    delay(500);
    // scanWifi();
    delay(500);
    connectWifi();
    delay(500);
    // startServer();
    delay(500);
    cameraConfig();
    delay(500);
    connectAWS();
    delay(500);
}



void loop(){
    server.handleClient();
    if (millis() - picturePreviousMillis > pictureInterval)
    {
      picturePreviousMillis = millis();
      publishMessage();
    }
    client.loop();

    delay(2);
    

}