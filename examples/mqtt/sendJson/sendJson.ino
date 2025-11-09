#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "config.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

unsigned long previousMillis = 0;

JsonDocument doc;
String jsonStr = "";

void setup() {
  Serial.begin(115200);
  
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PSWD);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(BROKER);

  if (!mqttClient.connect(BROKER, PORT)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // Load example data
  doc["location"]["lat"] = 0;
  doc["location"]["lng"] = 0;
  doc["speed"] = 10;
  doc["utc"] = "05/11/2025";
  doc["signal"] = false;

  // Serialize JSON
  serializeJson(doc, jsonStr);
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(TOPIC);
    Serial.println(jsonStr);

    mqttClient.beginMessage(TOPIC);
    mqttClient.print(jsonStr);
    mqttClient.endMessage();

    Serial.println();
  }
}