/*****************************
Test du wifi de l'ESP32
Mehdi Ben Djedidia 25/02/2021
*****************************/

#include <Arduino.h>
#include <WiFi.h>
#include <secret.h>

const char *SSID{SSID_SECRET};
const char *PASSWD{PASSWD_SECRET};
const uint8_t PIN_OUTPUT{5};

void setup()
{
  Serial.begin(9600L);
  pinMode(PIN_OUTPUT, OUTPUT);
  WiFi.mode(WIFI_STA);

  digitalWrite(PIN_OUTPUT, LOW);
  WiFi.begin(SSID, PASSWD);
}

void loop()
{
  if (WiFi.isConnected())
  {
    Serial.printf("Connecté");
    digitalWrite(PIN_OUTPUT, HIGH);
  };
}