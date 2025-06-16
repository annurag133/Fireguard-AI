#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include "DHT.h"

// WiFi credentials
#define WIFI_SSID "s20"
#define WIFI_PASSWORD "annuraggg"

// Firebase API credentials
#define API_KEY "AIzaSyCtbUHwK00WoEA24qEI6A2ZVwtDYHm4qIs"
#define DATABASE_URL "https://safetydevice-ad684-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "anuragtiw795@gmail.com"
#define USER_PASSWORD "anurag.133"

// DHT11 sensor settings
#define DHTPIN D2          // GPIO Pin where DHT11 is connected
#define DHTTYPE DHT11      // DHT 11 sensor type

// Smoke sensor pin
#define SMOKE_SENSOR_PIN A0  // Analog pin for smoke sensor

// Firebase and DHT objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;

// Callback function to monitor token generation status
void tokenStatusCallback(token_info_t info) {
  if (info.status == token_status_ready) {
    Serial.println("Token ready.");
  } else if (info.status == token_status_on_signing) {
    Serial.println("Signing in.");
  } else if (info.status == token_status_on_request) {
    Serial.println("Requesting token.");
  } else if (info.status == token_status_error) {
    Serial.printf("Token error: %s\n", info.error.message.c_str());
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin(); // Initialize DHT11 sensor

  // Initialize smoke sensor
  pinMode(SMOKE_SENSOR_PIN, INPUT);
  Serial.println("Smoke Sensor Initialized...");

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to Wi-Fi with IP: ");
  Serial.println(WiFi.localIP());

  // Firebase initialization
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // Define token status callback function

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int smokeLevel = analogRead(SMOKE_SENSOR_PIN);  // Read the smoke sensor value

  // Check if DHT readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Check if Firebase is ready and send data every 15 seconds
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Send temperature data to Firebase
    if (Firebase.setFloat(fbdo, "/sensorData/temperature", temperature)) {
      Serial.print("Temperature sent: ");
      Serial.println(temperature);
    } else {
      Serial.print("Failed to send temperature: ");
      Serial.println(fbdo.errorReason());
    }

    // Send humidity data to Firebase
    if (Firebase.setFloat(fbdo, "/sensorData/humidity", humidity)) {
      Serial.print("Humidity sent: ");
      Serial.println(humidity);
    } else {
      Serial.print("Failed to send humidity: ");
      Serial.println(fbdo.errorReason());
    }

    // Send smoke level data to Firebase
    if (Firebase.setInt(fbdo, "/sensorData/smokeLevel", smokeLevel)) {
      Serial.print("Smoke Level sent: ");
      Serial.println(smokeLevel);
    } else {
      Serial.print("Failed to send smoke level: ");
      Serial.println(fbdo.errorReason());
    }

    // Check for high smoke levels and display a warning if needed
    if (smokeLevel > 20) {  // Example threshold for high smoke levels
      Serial.println("Warning: High Smoke Detected!");
    }
  }

  delay(1000);  // Delay for 1 second before next reading
}
