#include <TinyGPS++.h>

#define RXD2 16
#define TXD2 17
#define PRINT_INTERVAL 1000

#define GPS_BAUD 9600

TinyGPSPlus gps;

HardwareSerial gpsSerial(2);
unsigned long lastPrintTime = 0; 

void setup() {
  Serial.begin(115200);
  
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("GPS Initialized");
}

void loop() {
  // Process incoming GPS data
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  // Check if a second has passed since the last data update
  if (millis() - lastPrintTime >= PRINT_INTERVAL) {
    lastPrintTime = millis(); // Reset the timer
    
    if (gps.location.isUpdated()) {
      Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("LONG: "); 
      Serial.println(gps.location.lng(), 6);
      Serial.print("SPEED (km/h) = "); 
      Serial.println(gps.speed.kmph()); 
      Serial.print("ALT (min)= "); 
      Serial.println(gps.altitude.meters());
      Serial.print("HDOP = "); 
      Serial.println(gps.hdop.value() / 100.0); 
      Serial.print("Satellites = "); 
      Serial.println(gps.satellites.value()); 
      Serial.print("Time in UTC: ");
      Serial.println(String(gps.date.year()) + "/" + String(gps.date.month()) + "/" + String(gps.date.day()) + "," + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
      Serial.println("");
    }
    else {
      Serial.println("GPS without signal");
    }
  }
}