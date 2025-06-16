#include <SoftwareSerial.h>
#include <TinyGPS++.h> 
#include <ESP8266WiFi.h>

#define gpsRxPin D7
#define gpsTxPin D6
SoftwareSerial neo6m(gpsTxPin, gpsRxPin);

TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  neo6m.begin(9600);

  Serial.println("GPS Data Initialization...");
}

void loop()
{
  // Capture GPS data
  smartdelay_gps(1000);
  
  // Check if valid GPS data is available
  if (gps.location.isValid()) 
  {
    // Store the Latitude and Longitude
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6); 
    
    // Print the values to Serial Monitor
    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);
  } 
  else 
  {
    // If the GPS location is invalid, print an error message
    Serial.println("Waiting for valid GPS data...");
  }
}

// Function to handle GPS data reading with a delay
static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}
