#include <Servo.h>

// Flame sensor pins
#define FLAME_FL D6
#define FLAME_FR D2
#define FLAME_BL D3
#define FLAME_BR D1

// Servo and Relay pins
#define SERVO_X_PIN D5
#define SERVO_Y_PIN D4
#define RELAY_PIN   D7

Servo servoX;
Servo servoY;

bool fireDetected = false;

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_FL, INPUT_PULLUP);
  pinMode(FLAME_FR, INPUT_PULLUP);
  pinMode(FLAME_BL, INPUT_PULLUP);
  pinMode(FLAME_BR, INPUT_PULLUP);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Pump OFF

  servoX.attach(SERVO_X_PIN, 500, 2400); // SG90 pulse range
  servoY.attach(SERVO_Y_PIN, 500, 2400);

  servoX.write(90); // Center
  servoY.write(90);
  delay(500);
}

void loop() {
  int fl = digitalRead(FLAME_FL);
  int fr = digitalRead(FLAME_FR);
  int bl = digitalRead(FLAME_BL);
  int br = digitalRead(FLAME_BR);

  if (!fireDetected && (fl == LOW || fr == LOW || bl == LOW || br == LOW)) {
    fireDetected = true;

    if (fl == LOW) {
      Serial.println("🔥 Fire at Front-Left");
      moveToFire(45, 135);  // Adjust based on real-world placement
    } else if (fr == LOW) {
      Serial.println("🔥 Fire at Front-Right");
      moveToFire(45, 45);
    } else if (bl == LOW) {
      Serial.println("🔥 Fire at Back-Left");
      moveToFire(145, 135);
    } else if (br == LOW) {
      Serial.println("🔥 Fire at Back-Right");
      moveToFire(135, 45);
    }

    delay(800);  // Let servo move
    digitalWrite(RELAY_PIN, LOW);  // Pump ON
    Serial.println("🚿 Pump ON");

  } else if (fireDetected && (fl == HIGH && fr == HIGH && bl == HIGH && br == HIGH)) {
    // Fire gone
    fireDetected = false;
    digitalWrite(RELAY_PIN, HIGH); // Pump OFF
    Serial.println("✅ Fire gone. Pump OFF");
    
    servoX.write(90);
    servoY.write(90);
  }

  delay(200);
}

void moveToFire(int xAngle, int yAngle) {
  servoX.write(xAngle);
  delay(300);
  servoY.write(yAngle);
  delay(300);
}
