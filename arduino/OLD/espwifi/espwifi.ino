#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

void connetWIFI(void);

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Nishant"
#define WIFI_PASSWORD "lconnect"

#define po D5
#define ne D6


// const char* WIFI_SSID="realme Pad X";
// const char* password = "abcd1234";


// Insert Firebase project API Key
#define API_KEY "_____________________"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "___________________.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//some importent variables
String sValue;
int value;


bool signupOK = false;


void connetWIFI() {
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println();
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.print("NodeMCU mac Address : ");
  Serial.println(WiFi.macAddress());
}

void dd(int quan){
  int x = quan / 20;
  int error = x * 2;
  quan -= error;
  int to_time = (quan * 1666);
      digitalWrite(po,HIGH);
      digitalWrite(ne,LOW);
      delay(to_time);
      digitalWrite(po,LOW);    
    
}

void setup() {
   // motor variable
  pinMode(po, OUTPUT);
  pinMode(ne, OUTPUT);

  Serial.begin(115200);
  connetWIFI();


  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // data updated
  updateData("0");

 
}


void readData() {
  if (Firebase.ready() && signupOK) {
    if (Firebase.RTDB.getString(&fbdo, "/vlue")) {
      if (fbdo.dataType() == "string") {
        sValue = fbdo.to<String>();
        value = sValue.toInt();
      }
    } else {
      Serial.println("error in reading");
      Serial.println(fbdo.errorReason());
    }
  }
}

void updateData(String a) {
  if (Firebase.ready() && signupOK) {
    if (Firebase.RTDB.setString(&fbdo, "/vlue", a)) {
      Serial.println("value updated!!");
    }
  }
}

void printData() {
  Serial.print("vlue :: ");
  Serial.print(value);
  Serial.println();
}


void quickDispence(int a) {
  if (a == -1) { 
    dd(50);
    Serial.println("quick despence 50ml");
  } else if (a == -2) {
    dd(50);
    Serial.println("quick despence 50ml");
  } else if (a == -3) {
    dd(75);
    Serial.println("quick despence 75ml");
  } 
  updateData("0");
}

void scrlDispence(int a) {
    dd(a);
    Serial.print("dispensing ");
    Serial.print(a);
    Serial.println("ml via scroll method in normal speed");
  updateData("0");
}

void loop() {
  readData();
  printData();
  if (value != 0) {
    if (value < 0) quickDispence(value);
    else scrlDispence(value);
  }
}
