#define TINY_GSM_MODEM_SIM800
#include <SoftwareSerial.h>
#include <TinyGsmClient.h>

#define SerialMon Serial // For serial monitor
SoftwareSerial SerialAT(D5, D6); // RX, TX (Modify as needed)

TinyGsm modem(SerialAT);

const char* phoneNumber = "+917226068792"; // Replace with your number

void setup() {
  SerialMon.begin(115200);
  delay(10);

  SerialAT.begin(9600); // Adjust baud rate as needed (4800 or 115200)
  delay(3000); // Adjust this delay for proper initialization

  modemInit(); // Faster AT-based initialization

  int signalQuality = modem.getSignalQuality();
  SerialMon.print("Signal Quality: ");
  SerialMon.println(signalQuality);

  if (modem.isNetworkConnected()) {
    SerialMon.println("Modem connected to network.");
    sendSMS(phoneNumber, "Hello from ESP8266 and SIM800L!");
  } else {
    SerialMon.println("Failed to connect to the network!");
    checkNetworkRegistration(); // Check registration status
  }
}

void loop() {
  // Nothing in the loop
}

void sendSMS(const char* number, const char* message) {
  SerialMon.print("Sending SMS...");

  if (modem.sendSMS(number, message)) {
    SerialMon.println("SMS sent successfully!");
  } else {
    SerialMon.println("Failed to send SMS.");
  }
}

void modemInit() {
  SerialMon.println("Initializing modem with AT commands...");

  // Attempt to restart the modem
  if (!modem.restart()) {
    SerialMon.println("Failed to restart modem! Trying again...");
    delay(3000); // Wait for 3 seconds before retrying
    if (!modem.restart()) {
      SerialMon.println("Failed to restart modem after retry!"); // Exiting if it fails twice
      return;
    }
  }

  // Continue with modem setup
  SerialMon.println("Modem initialized successfully.");
  
  // Set SMS mode
  modem.sendAT("+CMGF=1"); // Set SMS text mode
  delay(1000); // Wait for a moment to process

  SerialMon.println("Modem initialization completed.");
}

void checkNetworkRegistration() {
  SerialMon.println("Checking network registration...");
  uint8_t regStatus = modem.getRegistrationStatus(); // Get registration status as a number

  SerialMon.print("Registration Status: ");
  switch (regStatus) {
    case 1:
      SerialMon.println("Registered (Home network)");
      break;
    case 5:
      SerialMon.println("Registered (Roaming)");
      break;
    case 0:
      SerialMon.println("Not registered, searching for network...");
      break;
    case 2:
      SerialMon.println("Not registered, but searching...");
      break;
    case 3:
      SerialMon.println("Registration denied");
      break;
    default:
      SerialMon.println("Unknown registration status");
      break;
  }
}
