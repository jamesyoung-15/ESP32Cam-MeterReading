#ifndef MY_SECRETS_H
#define MY_SECRETS_H

/* Wifi Password */
#define WIFI_SSID "SSID"
#define WIFI_PASSWD "PASSWD"

/* AWS S3 Bucket */
const char S3_FOLDER_NAME[] = "";

/* AWS Credentials */

const char AWS_IOT_THINGNAME[] = "";

// Amazon Root CA 1
static const char AWS_IOT_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_IOT_DEVICE_CERT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_IOT_PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";

const char AWS_IOT_ENDPOINT[] = "****-ats.iot.us-east-1.amazonaws.com";

const char AWS_IOT_PUBLISH_TOPIC[] = "";

const char AWS_IOT_SUBSCRIBE_TOPIC[] = "";


/* AWS API */

const char AWS_REST_API[] = "https://*******.execute-api.us-east-1.amazonaws.com/***";

const char AWS_API_CERT[] =
"-----BEGIN CERTIFICATE-----\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"-----END CERTIFICATE-----";

#endif