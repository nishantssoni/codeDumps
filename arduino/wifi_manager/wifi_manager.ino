/*
This code connects an ESP8266 to a WiFi network using WiFiManager and sends
temperature and humidity data to a Google Sheets document via a Google Apps Script.
The onboard LED indicates the status of the ESP8266.

*/

#include <WiFiManager.h>
#include <WiFiClientSecure.h>

#define ON_Board_LED 2 // Pin number for the onboard LED

//----------------------------------------Host & httpsPort
const char* host = "script.google.com"; // Google Script host
const int httpsPort = 443; // HTTPS port number
//----------------------------------------

WiFiClientSecure client; // Create a WiFiClientSecure object

String GAS_ID = "AKfycbwbZK3TCSS1RZ60t73phWxYoqQ4R8pBrhoaLiJSoLJFheh6PbHbjk5WtlILRQfXafgh"; // Google Apps Script ID

void setupWiFiManager() {
  WiFi.mode(WIFI_STA); // Set WiFi mode to station
  WiFiManager wm;

  // Uncomment the following line to reset WiFi settings
  // wm.resetSettings();

  WiFi.forceSleepWake(); // Wake up WiFi
  WiFi.setSleepMode(WIFI_NONE_SLEEP); // Disable WiFi sleep mode

  wm.setConnectTimeout(180); // Set connection timeout
  wm.setConnectRetries(100); // Set connection retries
  if (wm.getWiFiIsSaved()) wm.setEnableConfigPortal(false);

  bool res;
  res = wm.autoConnect("AutoConnectAP"); // Connect to WiFi using WiFiManager

  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart(); // Restart ESP if connection fails
  } else {
    Serial.println("connected... :)"); // Print success message if connected
  }
}

void setup() {
  Serial.begin(115200); // Initialize serial communication

  setupWiFiManager(); // Set up WiFi connection using WiFiManager
    
  pinMode(ON_Board_LED, OUTPUT); // Set onboard LED pin as output
  digitalWrite(ON_Board_LED, HIGH); // Turn off onboard LED initially

  client.setInsecure(); // Set the client to insecure mode (no certificate validation)
}

void loop() {
  // If WiFi is disconnected, try to reconnect by restarting the ESP
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("wifi disconnected, connecting..");
    Serial.print(".");
    ESP.restart();
    delay(500);
  }

  // Simulated sensor readings for temperature and humidity
  int h = 56;
  float t = 43.4;
  
  // Check if sensor readings are valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from sensor!");
    delay(500);
    return;
  }

  // Print sensor readings to serial monitor
  String Temp = "Temperature : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";
  Serial.println(Temp);
  Serial.println(Humi);
  
  sendData(t, h); // Call the sendData function to send sensor readings to Google Sheets

  delay(5000); // Delay 5 seconds before repeating
}

// Function for sending data to Google Sheets
void sendData(float tem, int hum) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  // Prepare data for sending
  String string_temperature = String(tem);
  String string_humidity = String(hum, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;
  Serial.print("requesting URL: ");
  Serial.println(url);

  // Send HTTP GET request to Google Apps Script
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: ESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");

  // Check if the data was sent successfully
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Failed to send data");
  }

  Serial.print("reply was: ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
}
