#define RX2 16
#define TX2 17

#include <TinyGPSPlus.h>

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

void setup() {
  Serial.begin(115200);

  gpsSerial.begin(115200, SERIAL_8N1, RX2, TX2);
  Serial.println("GPS initialized");
}

void loop() {
  unsigned long start = millis();

  while(millis() - start < 1000) {
    while(gpsSerial.available() > 0)
      gps.encode(gpsSerial.read());

    if(gps.location.isUpdated()) {
      Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("LNG: ");
      Serial.println(gps.location.lng(), 6);
    }
  }
}
