#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"

// WiFi credentials
#define WIFI_SSID "s20"
#define WIFI_PASSWORD "12345678"

// Telegram Bot Token and Chat ID
#define BOT_TOKEN "7932488045:AAG9XpCh20kb4fqyoEqfSwLO_mXeIu2ndoQ"
#define CHAT_ID "1114081526"

// DHT11 sensor settings
#define DHTPIN D2          // GPIO Pin where DHT11 is connected
#define DHTTYPE DHT11      // DHT 11 sensor type

// Smoke sensor pin
#define SMOKE_SENSOR_PIN A0  // Analog pin for smoke sensor

// Threshold values
#define TEMP_THRESHOLD 29 // Temperature threshold for the alert (in Celsius)
#define SMOKE_THRESHOLD 20     // Smoke level threshold for the alert

// Telegram bot and WiFiClientSecure objects
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;

// Alert flags
bool tempAlertSent = false;  // Flag to indicate if temperature alert has been sent
bool smokeAlertSent = false;  // Flag to indicate if smoke alert has been sent

// Function to send a message to Telegram
void sendTelegramMessage(String message) {
  if (bot.sendMessage(CHAT_ID, message, "")) {
    Serial.println("Message sent to Telegram successfully.");
  } else {
    Serial.println("Error sending message to Telegram.");
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

  // Set up client for HTTPS communication
  client.setInsecure(); // Use insecure connection for now, disables SSL validation
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
  if (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0) {
    sendDataPrevMillis = millis();

    // Prepare message for Telegram (optional)
    String message = "Temperature: " + String(temperature) + "°C\n";
    message += "Humidity: " + String(humidity) + "%\n";
    message += "Smoke Level: " + String(smokeLevel);

    // Only send the readings to Telegram if desired
    // sendTelegramMessage(message); // Uncomment if you want to send readings

    // Check for high temperature and send alert if needed
    if (temperature >= TEMP_THRESHOLD && !tempAlertSent) {
      String tempAlert = "Alert: High Temperature Detected! (" + String(temperature) + "°C)";
      sendTelegramMessage(tempAlert);
      tempAlertSent = true;  // Set flag to prevent further alerts until reset
    }

    // Reset the alert if temperature drops below threshold
    if (temperature < TEMP_THRESHOLD) {
      tempAlertSent = false;  // Reset alert flag
    }

    // Check for high smoke levels and send alert if needed
    if (smokeLevel > SMOKE_THRESHOLD && !smokeAlertSent) {
      String smokeAlert = "Alert: High Smoke Level Detected! (" + String(smokeLevel) + ")";
      sendTelegramMessage(smokeAlert);
      smokeAlertSent = true;  // Set flag to prevent further alerts until reset
    }

    // Reset the smoke alert if the level goes below the threshold
    if (smokeLevel <= SMOKE_THRESHOLD) {
      smokeAlertSent = false;  // Reset alert flag
    }
  }

  delay(1000);  // Delay for 1 second before next reading
}
