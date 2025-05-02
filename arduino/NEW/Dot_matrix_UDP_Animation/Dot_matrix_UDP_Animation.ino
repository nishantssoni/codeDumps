#include "led.h"
#include <MD_MAX72xx.h>
#include <WiFi.h>  // Use the WiFi library for ESP32
#include <WiFiClient.h>
#include <WebServer.h>  // Updated to WebServer for ESP32
#include <WiFiUdp.h>

const char *ssid = "Nishant_hero";  // Change SSID
const char *password = "12345678";  // Change Password
const byte pinBuiltinLed = 2;       // Use GPIO 2 for built-in LED on ESP32

unsigned int localPort = 8888;       // Port from which we receive UDP packets
const int NTP_PACKET_SIZE = 48;      // Buffer size for received packets
char packetBuffer[NTP_PACKET_SIZE];  // Packet storage buffer
WiFiUDP udp;

MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::FC16_HW, 23, 18, 5, 1);
int speed_val = 50;

// Timing variables for animations
unsigned long previousMillis = 0;    // Stores the last time the animation was updated
const long animationInterval = 100;  // Time between animation updates (in milliseconds)

int counter = 1;           // To track animation state
int currentAnimation = 0;  // To track the current animation

int verbose = 0;

void setup() {
  Serial.begin(115200);
  setupLED();  // Initialize LED

  if (verbose) {
    // Configure WiFi
    Serial.print("Configuring access point...");
    Serial.println(ssid);
  }


  WiFi.mode(WIFI_OFF);                 // Turn off WIFI
  WiFi.mode(WIFI_AP);                  // Put WIFI in Access Point Mode
  WiFi.softAP(ssid, password, 11, 0);  // Start the access point

  IPAddress myIP = WiFi.softAPIP();
  if (verbose) {
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    Serial.println("HTTP server started");
    Serial.println("Starting UDP");
  }

  udp.begin(localPort);  // Initialize UDP

  if (verbose) {
    Serial.print("Local port: ");
    Serial.println(localPort);  // Print local port
  }
}

void loop() {
  // Check for UDP packets
  int packetSize = udp.parsePacket();
  if (packetSize) {
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    if (verbose) {
      Serial.println(packetBuffer);
    }
    String req = String(packetBuffer);

    // Check for the "stop" command
    // if (req.indexOf("stop") != -1) {
    //   counter++;  // Increment counter to change animation
    // }
    if (req.startsWith("animat_"))  //slider_
    {
      counter = format_string_anim(req);
    }
    if (req.startsWith("bright_"))  //slider_
    {
      setBrightness(format_string(req));
    }
    if (req.startsWith("speeed_"))  //slider_
    {
      setSpeed(format_string(req));
    }
  }

  // Update the current animation based on the counter
  currentAnimation = counter-1;  // Cycle through 0, 1, 2 for animations

  // Run the current animation in a non-blocking manner
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= animationInterval) {
    previousMillis = currentMillis;  // Update the last update time

    // Run the current animation
    switch (currentAnimation) {
      case 0:
        diagonalLine();
        break;
      case 1:
        blinkCorners();
        break;
      case 2:
        bouncingBall();
        break;
      case 3:
        movingLine();
        break;
      case 4:
        zigzagPattern();
        break;
      case 5:
        flashingSquare();
        break;
      case 6:
        spiralPattern();
        break;
      case 7:
        rainEffect();
        break;
      case 8:
        wavePattern();
        break;
      case 9:
        rotatingSquare();
        break;
      case 10:
        fireEffect();
        break;
      case 11:
        starfield();//
        break;
      case 12:
        plasmaEffect();
        break;
      case 13:
        fireworksEffect();
        break;
      case 14:
        sineWavePattern();
        break;
      case 15:
        cosineWavePattern();
        break;
      case 16:
        dualWavePattern();
        break;
      case 17:
        graphPattern();
        break;
      case 18:
        rippleWavePattern();
        break;
      case 19:
        rotatingCube();
        break;
      case 20:
        spiralEffect();
        break;
      case 21:
        tunnelEffect();
        break;
      case 22:
        waveEffect3d();
        break;
    }
  }
}