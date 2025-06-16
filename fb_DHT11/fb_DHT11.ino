#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>

// WiFi credentials
#define WIFI_SSID "s20"
#define WIFI_PASSWORD "12345678"

// Firebase credentials
#define FIREBASE_HOST "https://safetydevice-fc2a7-default-rtdb.asia-southeast1.firebasedatabase.app"  // Your Firebase database URL
#define FIREBASE_AUTH "AIzaSyCG7eluL5NMe6EA9zjbWFjT0Dy_0QS6_oA"  // Your Firebase API key

#define DHTPIN D2  // DHT11 sensor data pin connected to GPIO D2
#define DHTTYPE DHT11  // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

// Define Firebase objects
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Set Firebase configurations
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Connect to Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print values to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Create JSON data structure
  FirebaseJson json;
  json.set("temperature", temperature);
  json.set("humidity", humidity);

  // Send data to Firebase
  if (Firebase.pushJSON(firebaseData, "/dht11_sensor_data", json)) {
    Serial.println("Data sent to Firebase");
  } else {
    Serial.print("Error sending data: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(2000);  // Send data every 2 seconds
}
