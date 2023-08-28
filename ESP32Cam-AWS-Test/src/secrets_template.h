#ifndef SECRETS_H
#define SECRETS_H

/* Wifi Password */
#define WIFI_SSID "SSID"
#define WIFI_PASSWD "PASSWD"

/* AWS API */

// AWS API Gateway endpoint
const char AWS_REST_API[] = "https://*******.execute-api.us-east-1.amazonaws.com/***";

const char AWS_API_CERT[] =
"-----BEGIN CERTIFICATE-----\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
"-----END CERTIFICATE-----";

#endif