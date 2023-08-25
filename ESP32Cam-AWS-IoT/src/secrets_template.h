#ifndef SECRETS_H
#define SECRETS_H

/* Wifi Password */
#define WIFI_SSID "SSID"
#define WIFI_PASSWD "PASSWD"

/* AWS Credentials */
const char AWS_CA_CERT[] =
"-----BEGIN CERTIFICATE-----\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"-----END CERTIFICATE-----";

const char AWS_IOT_CERT[] =
"-----BEGIN CERTIFICATE-----\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"-----END CERTIFICATE-----";

const char AWS_IOT_PRIVATE_KEY[] = 
"-----BEGIN RSA PRIVATE KEY-----\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"-----END RSA PRIVATE KEY-----";


const char AWS_IOT_ENDPOINT[] = "xxxxxxxxxxxxx-ats.iot.us-east-1.amazonaws.com";


#endif