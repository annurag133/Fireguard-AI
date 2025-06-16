#include <DHT.h>  // Include the DHT library

#define DHTPIN D2     // Pin where the DHT sensor is connected
#define DHTTYPE DHT11 // DHT 11 sensor type

DHT dht(DHTPIN, DHTTYPE); // Initialize the DHT sensor

void setup() {
  Serial.begin(9600);          // Initialize serial communication
  Serial.println(F("DHT11 test!")); // Print a test message
  dht.begin();                // Start the DHT sensor
}

void loop() {
  delay(2000);               // Wait 2 seconds between readings

  float h = dht.readHumidity();     // Read humidity
  float t = dht.readTemperature();  // Read temperature

  if (isnan(h) || isnan(t)) {   // Check if the readings failed
    Serial.println(F("Failed to read from DHT Sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F(" °C"));
}
