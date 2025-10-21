/* RANSMITTER */

#include <TinyGPS++.h>
#include <ArduinoJson.h>

/* -------------------- CONFIG -------------------- */ 
#define GPS_RX 16    
#define GPS_TX 17    
#define LORA_RX 26   
#define LORA_TX 25   

#define GPS_BAUD 9600
#define LORA_BAUD 115200

#define LORA_TARGET_ADDRESS 2
#define NETWORK_ID 5

#define PRINT_INTERVAL 3000

/* -------------------- OBJECTS -------------------- */
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);   // UART2 for GPS
HardwareSerial loraSerial(1);  // UART1 for LoRa

JsonDocument doc;
unsigned long lastSend = 0;

/* -------------------- FUNCTIONS -------------------- */
void sendLoraCommand(String cmd) {
  loraSerial.println(cmd);
  delay(500);
  while (loraSerial.available())
    Serial.print((char)loraSerial.read());
}

void setupLoRa() {
  Serial.println("🛰️ Configuring LoRa module...");

  sendLoraCommand("AT+ADDRESS=1");
  delay(500);
  sendLoraCommand("AT+NETWORKID=" + String(NETWORK_ID));
  delay(500);
  sendLoraCommand("AT+BAND");       // Read current frequency
  delay(500);
  sendLoraCommand("AT+PARAMETER");  // Read parameters
  delay(500);
  sendLoraCommand("AT+MODE");       // Read mode
  delay(500);

  Serial.println("✅ LoRa ready");
}

void setupGPS() {
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("🧭 GPS initialized");
}

void buildJson() {
  doc.clear();

  if (gps.location.isValid()) {
    doc["location"]["lat"] = gps.location.lat();
    doc["location"]["lng"] = gps.location.lng();
    doc["speed"] = gps.speed.kmph();
    doc["utc"] = String(gps.date.year()) + "/" +
                 String(gps.date.month()) + "/" +
                 String(gps.date.day()) + "," +
                 String(gps.time.hour()) + ":" +
                 String(gps.time.minute()) + ":" +
                 String(gps.time.second());
    doc["signal"] = true;
  }
  else {
    doc["location"]["lat"] = nullptr;
    doc["location"]["lng"] = nullptr;
    doc["speed"] = nullptr;
    doc["utc"] = nullptr;
    doc["signal"] = false;
  }
}

void sendJsonOverLoRa() {
  String jsonStr;
  serializeJson(doc, jsonStr);

  String msgLen = String(jsonStr.length());
  String command = "AT+SEND=" + String(LORA_TARGET_ADDRESS) + "," + msgLen + "," + jsonStr;

  Serial.println("📤 Sending JSON:");
  Serial.println(jsonStr);
  sendLoraCommand(command);
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Initialize UARTs
  loraSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX, LORA_TX);
  setupLoRa();

  setupGPS();

  Serial.println("🚀 System ready to transmit data");
}

void loop() {
  // Read incoming GPS data
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (millis() - lastSend >= PRINT_INTERVAL) {
    lastSend = millis();

    buildJson();
    sendJsonOverLoRa();
  }
}
