#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Firebase API Key and Database URL
#define API_KEY "https://YOUR_API_KEY.firebaseio.com/"
#define DATABASE_URL "YOUR_DATABASE_URL"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth; // Firebase authentication object
FirebaseConfig config; // Firebase configuration object

unsigned long sendDataPrevMillis = 0; // Variable to store the last time data was sent

const int ledPin = 2; // Pin connected to the LED

void setup() 
{
  pinMode(ledPin, OUTPUT); // Set the LED pin as output
  digitalWrite(ledPin, LOW); // Turn off the LED initially

  WiFi.mode(WIFI_STA); // Set WiFi mode to station
  Serial.begin(115200); // Initialize serial communication
  
  WiFiManager wm;

  // Uncomment the following line to reset settings and wipe stored credentials for testing
  // wm.resetSettings();

  bool res;
  
  // Try to connect to WiFi using WiFiManager
  res = wm.autoConnect("AutoConnectAP"); // Anonymous AP
  // res = wm.autoConnect("AutoConnectAP", "password"); // Password protected AP

  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart(); // Restart the ESP if the connection fails
  } 
  else {  
      Serial.println("Connected...");
  }

  // Assign the API key (required)
  config.api_key = API_KEY;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  // Set the buffer size for Firebase data object
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  // Initialize Firebase
  Firebase.begin(&config, &auth);

  // Set the number of decimal places for double values
  Firebase.setDoubleDigits(5);

  // Set the timeout for server response
  config.timeout.serverResponse = 10 * 1000;
}

void loop() {
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis(); // Update the last time data was sent

    int ledState; // Variable to store the state of the LED
    // Get the value of "/led" from the Firebase Realtime Database
    if (Firebase.RTDB.getInt(&fbdo, "/led", &ledState)) {
      Serial.println(ledState); // Print the LED state to the serial monitor
      digitalWrite(ledPin, ledState); // Set the LED state
    } else {
      Serial.println(fbdo.errorReason().c_str()); // Print the error reason if the operation fails
    }
  }
}
