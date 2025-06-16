#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>

#define gpsRxPin D1
#define gpsTxPin D2
SoftwareSerial neo6m(gpsTxPin, gpsRxPin);

TinyGPSPlus gps;

const char *ssid = "s20";  // WiFi name
const char *password = "12345678"; // WiFi password

String GMAP_API_KEY = "AIzaSyAyYkFvCZkUhUsBJIDA9oJXeZ1QXbaEAak";

WiFiServer server(80);

String html;

void setup() {
    Serial.begin(115200);
    Serial.println();
    neo6m.begin(9600);

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
    smartdelay_gps(1000);

    // Check if GPS location is valid
    if (gps.location.isValid()) {
        // Storing Latitude and Longitude
        String latitude = String(gps.location.lat(), 6);
        String longitude = String(gps.location.lng(), 6);

        // Send to Serial Monitor for Debugging
        Serial.print("LAT: ");
        Serial.print(latitude);
        Serial.print(" LONG: ");
        Serial.println(longitude);

        // Listen for incoming clients
        WiFiClient client = server.available();
        if (client) {
            Serial.println("new client");
            String currentLine = ""; // Hold incoming data from the client
            while (client.connected()) {
                if (client.available()) { // If there's client data
                    char c = client.read(); // Read a byte
                    if (c == '\n') { // Check for newline character
                        if (currentLine.length() == 0) { // End of the client HTTP request
                            // Prepare HTML response
                            html = "<!DOCTYPE html>";
                            html += "<html lang='en'>";
                            html += "<head>";
                            html += "<meta charset='UTF-8'>";
                            html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
                            html += "<meta http-equiv='X-UA-Compatible' content='ie=edge'>";
                            html += "<title>My Google Map</title>";
                            html += "<style>#map{height:400px;width:100%;}</style>";
                            html += "</head>";
                            html += "<body>";
                            html += "<h1>My Google Map</h1>";
                            html += "<div id='map'></div>";
                            html += "<script>";
                            html += "var map;";
                            html += "var marker;";
                            html += "var INTERVAL = 5000;";
                            html += "function initMap() {";
                            html += "var options = {";
                            html += "zoom: 16,";
                            html += "center: {lat: " + latitude + ", lng: " + longitude + "},";
                            html += "mapTypeId: google.maps.MapTypeId.ROADMAP,";
                            html += "};";
                            html += "map = new google.maps.Map(document.getElementById('map'), options);";
                            html += "}";
                            html += "function getMarkers() {";
                            html += "var newLatLng = new google.maps.LatLng(" + latitude + ", " + longitude + ");";
                            html += "marker = new google.maps.Marker({";
                            html += "position: newLatLng,";
                            html += "map: map";
                            html += "});";
                            html += "}";
                            html += "window.setInterval(getMarkers, INTERVAL);";
                            html += "</script>";
                            html += "<script async defer src='https://maps.googleapis.com/maps/api/js?key=" + GMAP_API_KEY + "&callback=initMap'></script>";
                            html += "</body></html>";

                            client.print(html);
                            client.println();
                            break;
                        } else {
                            currentLine = ""; // Reset current line
                        }
                    } else if (c != '\r') {
                        currentLine += c; // Add to current line
                    }
                }
            }
            client.stop();
            Serial.println("client disconnected");
        }
    } else {
        Serial.println("GPS location not valid.");
    }
}

static void smartdelay_gps(unsigned long ms) {
    unsigned long start = millis();
    do {
        while (neo6m.available()) {
            gps.encode(neo6m.read());
            if (gps.location.isUpdated()) {
                // Add debugging info here
                Serial.print("GPS data updated: ");
                Serial.print("LAT: ");
                Serial.print(gps.location.lat(), 6);
                Serial.print(" LONG: ");
                Serial.println(gps.location.lng(), 6);
            }
        }
    } while (millis() - start < ms);
}
