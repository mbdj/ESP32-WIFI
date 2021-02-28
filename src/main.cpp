/*****************************
Test du wifi de l'ESP32
Mehdi Ben Djedidia 25/02/2021
*****************************/

#include <Arduino.h>
#include <WiFi.h>
#include <secret.h>
#include <WebServer.h>
#include <UnbouncedButton.h>

const char *SSID{SSID_SECRET};
const char *PASSWD{PASSWD_SECRET};
const uint8_t PIN_LED{5};
UnbouncedButton Bouton{4, INPUT_PULLDOWN};

WebServer server;

void setup()
{
  Serial.begin(9600L);
  Serial.printf("--- ESP 32 ---");

  pinMode(PIN_LED, OUTPUT);
  WiFi.mode(WIFI_STA);

  digitalWrite(PIN_LED, LOW);
  WiFi.begin(SSID, PASSWD);

  // On fait clignoter la LED tant que la connexion WIFI n'est pas établie
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    delay(200);
  }

  // connecté au WIFI
  digitalWrite(PIN_LED, HIGH);

  // définition des routes pour le serveur web
  //server.on("/");
  // démarrage du serveur web
  server.begin();
}

void loop()
{
  // On change l'état de la LED en pressant le bouton
  if (Bouton.buttonPressed())
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
}