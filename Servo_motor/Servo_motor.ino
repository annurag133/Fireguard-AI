#include <ESP8266WiFi.h>  // If you're using WiFi features
#include <Servo.h>        // After installing ESP8266Servo

Servo myservo;

void setup() {
  myservo.attach(D4); // or use GPIO pin number, like 2
}

void loop() {
  myservo.write(0);    // 0 degrees
  delay(1000);
  myservo.write(90);   // 90 degrees
  delay(1000);
  myservo.write(180);  // 180 degrees
  delay(1000);
}
