/* RECEIVER */
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include "config.h"

#define TX 17
#define RX 16

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

unsigned long previousMillis = 0;

String msg;
JsonDocument msgPayload;    
JsonDocument receivedPackage;
String jsonStr = "";

void sendMsg(String msg) {
  Serial2.println(msg);
  delay(500);
  while (Serial2.available())
    Serial.print(char(Serial2.read()));
}

void printJsonFromMsg() {
  Serial.print("ID: ");
  Serial.println(msgPayload["id"].as<String>());
  Serial.print("Latitude: ");
  Serial.println(msgPayload["localitation"]["lat"].as<float>());
  Serial.print("Longitude: ");
  Serial.println(msgPayload["localitation"]["lng"].as<float>());
}

void printLoraMetadata() {
  String sender = receivedPackage["sender"];
  String length = receivedPackage["length"];
  String rssi = receivedPackage["RSSI"];
  String snr = receivedPackage["SNR"];

  Serial.println("📡 LoRa metadata:");
  Serial.print("  Sender: "); Serial.println(sender);
  Serial.print("  Length: "); Serial.println(length);
  Serial.print("  RSSI: "); Serial.println(rssi);
  Serial.print("  SNR: "); Serial.println(snr);
}

// ✅ Builds the final JSON combining metadata + payload
void buildReceivedPackageJson() {
  // Extract LoRa metadata
  int firstComma = msg.indexOf(',');
  int secondComma = msg.indexOf(',', firstComma + 1);
  int lastComma = msg.lastIndexOf(',');
  int secondLastComma = msg.lastIndexOf(',', lastComma - 1);

  String sender = msg.substring(5, firstComma);
  String length = msg.substring(firstComma + 1, secondComma);
  String rssi = msg.substring(secondLastComma + 1, lastComma);
  String snr = msg.substring(lastComma + 1);

  receivedPackage["sender"] = sender;
  receivedPackage["length"] = length;
  receivedPackage["RSSI"] = rssi;
  receivedPackage["SNR"] = snr;
  receivedPackage["payload"] = msgPayload;

  printLoraMetadata();

  Serial.println("🧩 Final combined JSON:");
  serializeJsonPretty(receivedPackage, Serial);
  Serial.println();
}

void parseJsonFromMsg(String msg) {
  int start = msg.indexOf('{');
  int end = msg.lastIndexOf('}');

  if (start == -1 || end == -1 || end <= start) {
    Serial.println("No JSON detected in message");
    return;
  }

  String jsonPart = msg.substring(start, end + 1);

  Serial.println("JSON detected:");
  Serial.println(jsonPart);

  // Parse JSON payload
  DeserializationError error = deserializeJson(msgPayload, jsonPart);
  if (error) {
    Serial.print("❌ JSON parsing error: ");
    Serial.println(error.c_str());
    return;
  }

  Serial.println("✅ Valid JSON parsed");
  printJsonFromMsg();

  // Build the final combined JSON
  buildReceivedPackageJson();
}

// WIFI & MQTT
void ensureConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    WiFi.disconnect();
    WiFi.reconnect();

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
  
    if (WiFi.status() == WL_CONNECTED)
      Serial.println("✅ WiFi connected");
    else {
      Serial.println("❌ WiFi connection failed");
      return;
    }
  }

  if (!mqttClient.connected()) {
    Serial.println("MQTT disconnected. Attempting to reconnect...");
    
    if (mqttClient.connect(BROKER, PORT)) {
      Serial.println("✅ MQTT Conneced");
    } else {
      Serial.print("❌ MQTT connection failed");
      Serial.println(mqttClient.connectError());
      delay(1000); // Esperar un poco antes de continuar el loop
    }
  }
}

void setupWifi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PSWD);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);

  delay(3000);
  Serial.println("Setting LORA antenna parameters...");
  delay(1000);
  sendMsg("AT+ADDRESS=2");
  delay(1000);
  sendMsg("AT+NETWORKID=5");
  delay(1000);
  sendMsg("AT+BAND");
  delay(1000);
  sendMsg("AT+PARAMETER");
  delay(1000);
  sendMsg("AT+MODE");
  delay(1000);

  Serial.println("Receiver ready ✅");

  setupWifi();
  ensureConnection();
}

void loop() {
  ensureConnection();

  if(mqttClient.connected())
      mqttClient.poll();

  while (Serial2.available())
    msg = Serial2.readString();

  msg.trim();

  if (msg.startsWith("+RCV")) {
    Serial.println("Message received:");
    Serial.println(msg);

    parseJsonFromMsg(msg);
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    serializeJson(receivedPackage, jsonStr);

    Serial.print("Sending message to topic: " );
    Serial.println(TOPIC);  
    Serial.println(jsonStr);

    mqttClient.beginMessage(TOPIC);
    mqttClient.print(jsonStr);
    mqttClient.endMessage();

    Serial.println();
  } 

  delay(1000);
}