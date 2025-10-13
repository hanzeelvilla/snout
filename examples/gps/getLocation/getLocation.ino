#define RX2 16
#define TX2 17

#include <TinyGPSPlus.h>

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(115200);

  gpsSerial.begin(115200, SERIAL_8N1, RX2, TX2);
  Serial.println("GPS initialized");
}

void loop() {
  while(gpsSerial.available() > 0)
    gps.encode(gpsSerial.read());

  unsigned long currentMillis = millis();

  // Read data each "interval"
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if(gps.location.isValid()) {
      Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("LNG: ");
      Serial.println(gps.location.lng(), 6);
    }
    else
      Serial.println("GPS without signal");
  }    
}
