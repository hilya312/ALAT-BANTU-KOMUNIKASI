#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

const char* ssid = "beibeik";            // Replace with your network SSID
const char* password = "030102CAPRI";    // Replace with your network password
const char* serverName2 = "http://hilya.site/iot/getdata.php"; 
#define RELAY_PIN 15
unsigned long previousMillis = 0;
const long interval = 2000; // Interval in milliseconds (2 seconds)
bool relayState = false;

void setup() {
    Serial.begin(115200);  // Ensure Serial Monitor is set to 115200
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    Serial.print("Connecting to: ");
    Serial.println(serverName2);

    http.begin(client, serverName2);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode); 
      Serial.println(payload); 
      int rly = payload.toInt();
      if (rly == 0) {
       digitalWrite(RELAY_PIN, LOW);
      } else if (rly == 1) {
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            // Activate relay for 2 seconds
            digitalWrite(RELAY_PIN, LOW);
            Serial.println("Relay inactive");
            delay(2000); // Delay 2 seconds
            digitalWrite(RELAY_PIN, HIGH);
            Serial.println("Relay active");
          }
      }
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }  
  delay(100); // Small delay for stability
}
