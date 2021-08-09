/*****************************
Test du wifi de l'ESP32
Mehdi Ben Djedidia 25/02/2021
*****************************/

#include <Arduino.h>
#include <WiFi.h>
#include <secret.hpp>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <UnbouncedButton.h>

const char *SSID{SSID_SECRET};
const char *PASSWD{PASSWD_SECRET};
const uint8_t PIN_LED{5};
const uint8_t PIN_BUTTON{4};
UnbouncedButton Bouton{PIN_BUTTON, INPUT_PULLDOWN, 50};

StaticJsonDocument<200> jsonDocument;
char jsonBuffer[200];

WebServer server;

/**
 * @brief Réponse serveur json pour obtenir l'état de la LED (ON / OFF)
 */
void ledState()
{
  jsonDocument.clear();
  jsonDocument["state"] = digitalRead(PIN_LED) ? "on" : "off";
  serializeJson(jsonDocument, jsonBuffer);
  server.send(200, "application/json", jsonBuffer);
}

/**
 * @brief change l'état de la LED comme le bouton
 * 
 */
void buttonPostLedToggle()
{
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  digitalWrite(PIN_LED, !digitalRead(PIN_LED));
  server.send(200, "application/json", digitalRead(PIN_LED) == HIGH ? "{\"state\":\"on\"}" : "{\"state\":\"off\"}");
}

/**
 * @brief active ou désactive la LED avec un argument /ledSet/state?state=on ou /button/set?state=off
 * 
 */
void buttonPutLedSet()
{
  String argState = server.arg("state");
  deserializeJson(jsonDocument, argState);

  if (argState == "on")
  {
    digitalWrite(PIN_LED, HIGH);
    server.send(200, "application/json", "{\"state\":\"on\"}");
  }
  else if (argState == "off")
  {
    digitalWrite(PIN_LED, LOW);
    server.send(200, "application/json", "{\"state\":\"off\"}");
  }
  else
    // erreur 400 : Bad Request
    server.send(400, "application/json", "{}");
}

/**
 * @brief initialisation
 * 
 */
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

  //==========================================
  // définition des routes pour le serveur web
  //==========================================
  // GET /ledstate retourne l'état de la lampe (ON ou OFF)
  server.on("/ledState", ledState);
  // POST /button active ou désactive la LED
  server.on("/ledToggle", HTTP_POST, buttonPostLedToggle);
  // PUT /button active ou désactive la LED avec un argument /ledSet?state=on ou off
  server.on("/ledSet", HTTP_PUT, buttonPutLedSet);

  // démarrage du serveur web
  server.begin();
}

/**
 * @brief loop
 * 
 */
void loop()
{
  // On change l'état de la LED en pressant le bouton
  if (Bouton.buttonPressed())
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));

  // traitement d'une requête d'un client du serveur web
  server.handleClient();
}