#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* for OLED display */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


/* OLED setup */
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
/* ---------------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------------------- */
/* oled initialize and all bitmaps */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int speaker = D3;

unsigned long callStartTime = 0;  // Store call start time
bool isCallActive = false;        // Track call status
String curr_call_msg = "Unknown";
unsigned long previousMillis = 0;     // Store time for non-blocking counter
const long interval = 1000;           // 1 second update interval

const char *ssid = "ESP8266_AP";
const char *password = "12345678";

// Create a web server on port 80
ESP8266WebServer server(80);

void handleGOBCRoute() {
  String number = server.arg("number");
  String message = server.arg("message");

  Serial.print("Received Number: ");
  Serial.println(number);
  Serial.print("Received Message: ");
  Serial.println(message);

  if(message == "incoming"){
    call_incoming(number.c_str());
  }
  else if(message == "started"){
    call_started(number.c_str());
  }
  else if(message == "ended"){
    call_ended(number.c_str());
  }
  else{
    msg_received(message.c_str());
  }

  String response = "Received number: " + number + "\nMessage: " + message;
  server.send(200, "text/plain", response);
}

void handleRoot() {
  server.send(200, "text/html", "<h1>ESP8266 Message Receiver</h1><p>Use /gobc?number=123&message=hello</p>");
}

void setup() {
  Serial.begin(115200);
  
  // Start ESP8266 in AP mode
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Connect to: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(IP);

  // Define routes
  server.on("/", handleRoot);
  server.on("/gobc", handleGOBCRoute);

  // Start web server
  server.begin();
  Serial.println("Web server started");

pinMode(speaker, OUTPUT);
  // my setup
  //display begins
  connectDisplay();
  initDisplay();
  track1();
  track1();
  track1();
  track1();
  track1();
  track1();

  
}

void loop() {
  server.handleClient();
  if (isCallActive) {
    showCallDuration();  // Non-blocking live counter
  }
}


void connectDisplay() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();
}
void initDisplay() {
  // display.clearDisplay();
  // display.drawBitmap(0, 0, init_logo, 128, 64, WHITE);
  // display.display();
  // delay(2000);
  displayMessage("Sbike", 2, 15, 0);
}

void playTone(int pin, int duration, int after_duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  delay(after_duration);
}


void track1() {
  playTone(speaker, 10, 5);
  playTone(speaker, 20, 15);
  playTone(speaker, 20, 15);
  playTone(speaker, 10, 5);
}

void track2() {
  playTone(speaker, 20, 100);
  playTone(speaker, 40, 10);
  playTone(speaker, 40, 10);
  playTone(speaker, 20, 100);
}

void track3() {
  playTone(speaker, 30, 10);
  playTone(speaker, 10, 100);
  playTone(speaker, 30, 10);
}

void track4() {
  playTone(speaker, 1000, 100);
  playTone(speaker, 5, 5);
  playTone(speaker, 30, 50);
  playTone(speaker, 20, 20);
  playTone(speaker, 20, 20);
}

void displayMessage(const char* message, int textSize, int x, int y) {
  display.clearDisplay();
  display.setTextSize(textSize);  // Set text size based on the argument
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.println(message);  // Display the message based on the argument
  display.display();
}

void call_incoming(const char* message) {
  track1();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);  // Set text size based on the argument
  display.setTextColor(SSD1306_WHITE);
  display.println("incoming call");
  display.println(message);  // Display the message based on the argument
  display.display();
}
void call_started(const char* message) {
  callStartTime = millis();  // Record the time when call starts
  isCallActive = true;
  curr_call_msg = message;
  track2();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);  // Set text size based on the argument
  display.setTextColor(SSD1306_WHITE);
  display.println("on call");
  display.println(message);  // Display the message based on the argument
  display.display();
}
void call_ended(const char* message) {
  isCallActive = false;
  callStartTime = 0;
  curr_call_msg = "Unknown";
  previousMillis = 0;     // Store time for non-blocking counte
  track3();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);  // Set text size based on the argument
  display.setTextColor(SSD1306_WHITE);
  display.println("call ended");
  display.println(message);  // Display the message based on the argument
  display.display();
}
void msg_received(const char* message) {
  track3();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);  // Set text size based on the argument
  display.setTextColor(SSD1306_WHITE);
  display.println("msg_received");
  display.println(message);  // Display the message based on the argument
  display.display();
}

void showCallDuration() {
    unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    unsigned long elapsedTime = (currentMillis - callStartTime) / 1000;  // Convert to seconds
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("On call");
    display.println(curr_call_msg);
    display.print("Duration: ");
    display.print(elapsedTime);
    display.println(" sec");
    display.display();
  }
}
