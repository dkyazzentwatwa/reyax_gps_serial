#include <HardwareSerial.h>
#include <TinyGPSPlus.h>  // Excellent library for parsing NMEA data

#define RXPin 16
#define TXPin 17

HardwareSerial GnssSerial(1);
TinyGPSPlus gps;  // Parser object

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("GPS started!");
  GnssSerial.begin(115200, SERIAL_8N1, RXPin, TXPin);
}

void loop() {
  // Flag to check if new data was processed
  bool newData = false;

  // Feed data to the parser
  while (GnssSerial.available()) {
    if (gps.encode(GnssSerial.read())) {
      newData = true;
    }
  }

  // If new valid GPS data is available, print it
  if (newData && gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
    Serial.print("Time: ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
    delay(5000);  // Wait before checking again
  }

  // Display a warning if no GPS data has been processed after 5 seconds
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS data received: check wiring");
    delay(1000);
  }

  delay(500);  // Small delay to prevent CPU overuse
}