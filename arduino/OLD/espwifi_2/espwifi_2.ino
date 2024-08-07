#include <ESP8266WiFi.h>                 
#include <FirebaseArduino.h>                 
 
#define FIREBASE_HOST "________________.firebaseio.com"      
#define FIREBASE_AUTH "_______________________"            
#define WIFI_SSID "Nishant"                                  
#define WIFI_PASSWORD "73456"            
                                 
 
String fireStatus = "";                                                     // led status received from firebase  
                                                              
void setup() 
{
  Serial.begin(9600);
  delay(1000);                   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                               
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                  // connect to firebase
  Firebase.setString("system", "5");                       //send initial string of led status
}
 
void loop() 
{
  fireStatus = Firebase.getString("LED_STATUS");                                      // get ld status input from firebase
  // if (fireStatus == "ON") 
  // {                                                          // compare the input of led status received from firebase
  //   Serial.println("Led Turned ON");                                                        
  //   digitalWrite(led, HIGH);                                                         // make external led ON
  // } 
  // else if (fireStatus == "OFF") 
  // {                                                  // compare the input of led status received from firebase
  //   Serial.println("Led Turned OFF");
  //   digitalWrite(led, LOW);                                                         // make external led OFF
  // }
  // else 
  // {
  //   Serial.println("Command Error! Please send ON/OFF");
  // }
  Serial.println(fireStatus);
  delay(2000);
}