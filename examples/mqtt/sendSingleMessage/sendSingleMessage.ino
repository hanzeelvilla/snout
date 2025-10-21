#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "config.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

unsigned long previousMillis = 0;
String message = "Hello Snout 🐶";

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
    Serial.println(message);

    mqttClient.beginMessage(TOPIC);
    mqttClient.print(message);
    mqttClient.endMessage();

    Serial.println();
  }
}
