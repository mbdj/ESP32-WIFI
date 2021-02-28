/*****************************
Test du wifi de l'ESP32
Mehdi Ben Djedidia 25/02/2021
*****************************/

#include <Arduino.h>
#include <WiFi.h>
#include <secret.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <UnbouncedButton.h>

const char *SSID{SSID_SECRET};
const char *PASSWD{PASSWD_SECRET};
const uint8_t PIN_LED{5};
const uint8_t PIN_BUTTON{4};
UnbouncedButton Bouton{PIN_BUTTON, INPUT_PULLDOWN};

StaticJsonDocument<200> jsonDocument;
char jsonBuffer[200];

WebServer server;

/**
 * @brief Réponse serveur json pour obtenir l'état de la LED (ON / OFF)
 */
void etatLED()
{
  jsonDocument.clear();
  jsonDocument["LEDstate"] = digitalRead(PIN_LED) ? "ON" : "OFF";
  serializeJson(jsonDocument, jsonBuffer);
  server.send(200, "application/json", jsonBuffer);
}

void buttonPost()
{
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  digitalWrite(PIN_LED, ! digitalRead(PIN_LED));
  server.send(200, "application/json", "{}");
}

void setup()
{
  Serial.begin(9600L);
  Serial.println("--- ESP 32 ---");

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
  Serial.println("IP : " + WiFi.localIP().toString());

  // définition des routes pour le serveur web
  //
  // GET /ledstate retourne l'état de la lampe (ON ou OFF)
  server.on("/ledState", etatLED);
  // POST /button active ou désactive la LED
  server.on("/button", HTTP_POST, buttonPost);
  // démarrage du serveur web
  server.begin();
}

void loop()
{
  // On change l'état de la LED en pressant le bouton
  if (Bouton.buttonPressed())
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));

  // traitement d'une requête d'un client du serveur web
  server.handleClient();
}