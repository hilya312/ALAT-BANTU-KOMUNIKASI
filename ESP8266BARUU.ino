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
const char* serverName1 = "http://hilya.site/iot/setdata_esp8266.php"; 
String apiKeyValue = "user"; // Access key for storing data on the web server
unsigned long lastMillis1 = 0;
long interval1 = 10 * 1000; // Interval in milliseconds (10 seconds)
int relay = 0;

void setup() {
    Serial.begin(9600);
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
    unsigned long currentMillis = millis();
    if ((currentMillis - lastMillis1) > interval1) { 
        sendData();
        lastMillis1 = currentMillis;
    }
    readMic();
    delay(10);
}

void sendData() {
    if (WiFi.status() == WL_CONNECTED) {          
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverName1);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String httpRequestData = "api_key=" + apiKeyValue + "&relay=" + String(relay);
        int httpResponseCode = http.POST(httpRequestData);
        http.end();
        Serial.println(httpResponseCode);
    }
}

void readMic() {
    // Read the analog value from the microphone
    int sensorValue = analogRead(A0);
    Serial.print("data = ");
    Serial.println(sensorValue);

    // Control the relay based on the sensor value
    if (sensorValue > 900) {
        relay = 1;
    } else {
        relay = 0;
    }
}
