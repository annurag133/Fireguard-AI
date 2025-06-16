int sensorPin = A0;  // Define the analog pin for the smoke sensor

void setup() {
  Serial.begin(9600);  // Start serial communication at 115200 baud
  delay(1000);           // Small delay for setup
  Serial.println("Smoke Sensor Initialized...");
}

void loop() {
  int sensorValue = analogRead(sensorPin);  // Read the analog value from the smoke sensor
  Serial.print("Smoke Level: ");
  Serial.println(sensorValue);  // Print the smoke level

  // You can add thresholds here for logging or sending alerts
  if(sensorValue > 20) {  // Example threshold for high smoke levels
    Serial.println("Warning: High Smoke Detected!");
    // You can trigger a cloud event or send a notification here
  }
  
  delay(1000);  // Delay for 1 second before next reading
}
