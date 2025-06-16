#include <TinyGPS++.h>

// Create a TinyGPSPlus object
TinyGPSPlus gps;

volatile float minutes, seconds;
volatile int degree, secs, mins;

void setup() {
  Serial.begin(9600);  // Start communication with the Serial Monitor
  Serial1.begin(9600); // Start communication with the GPS module (Hardware UART)
}

void loop() {
  smartDelay(1000);
  double lat_val, lng_val, alt_m_val;
  uint8_t hr_val, min_val, sec_val;
  bool loc_valid, alt_valid, time_valid;
  
  lat_val = gps.location.lat(); // Get latitude
  loc_valid = gps.location.isValid(); // Check if location data is valid
  lng_val = gps.location.lng(); // Get longitude
  alt_m_val = gps.altitude.meters(); // Get altitude in meters
  alt_valid = gps.altitude.isValid(); // Check if altitude data is valid
  hr_val = gps.time.hour(); // Get hour
  min_val = gps.time.minute(); // Get minutes
  sec_val = gps.time.second(); // Get seconds
  time_valid = gps.time.isValid(); // Check if time data is valid

  // Print location data
  if (!loc_valid) {
    Serial.println("Latitude : *****");
    Serial.println("Longitude : *****");
  } else {
    DegMinSec(lat_val);
    Serial.print("Latitude in Decimal Degrees: ");
    Serial.println(lat_val, 6);
    Serial.print("Latitude in Degrees Minutes Seconds: ");
    Serial.print(degree); Serial.print("\t");
    Serial.print(mins); Serial.print("\t");
    Serial.println(secs);
    
    DegMinSec(lng_val);
    Serial.print("Longitude in Decimal Degrees: ");
    Serial.println(lng_val, 6);
    Serial.print("Longitude in Degrees Minutes Seconds: ");
    Serial.print(degree); Serial.print("\t");
    Serial.print(mins); Serial.print("\t");
    Serial.println(secs);
  }

  // Print altitude data
  if (!alt_valid) {
    Serial.println("Altitude : *****");
  } else {
    Serial.print("Altitude: ");
    Serial.println(alt_m_val, 6);
  }

  // Print time data
  if (!time_valid) {
    Serial.println("Time : *****");
  } else {
    char time_string[32];
    sprintf(time_string, "Time : %02d/%02d/%02d \n", hr_val, min_val, sec_val);
    Serial.print(time_string);
  }
}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (Serial1.available()) {
      gps.encode(Serial1.read());  // Use Serial1 for hardware UART
    }
  } while (millis() - start < ms);
}

void DegMinSec(double tot_val) {
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
