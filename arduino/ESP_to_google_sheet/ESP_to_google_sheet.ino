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

String GAS_ID = "_________________"; // Google Apps Script ID

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


// Subroutine for sending data to Google Sheets
// sending a large number of data
void sendData(float* dur, int* err, int size) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);

  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Processing data and sending data
  String err_1 = String(err[0]);
  // String string_temperature =  String(tem, DEC);
  String dur_1 = String(dur[0]);
  String url = "/macros/s/" + GAS_ID + "/exec?user_id=" + uid + "&examiner_id=" + eid;
  for (int i = 0; i < 10; i++) {
      url += "&e" + String(i+1) + "=" + String(errors[i]);
      url += "&d" + String(i+1) + "=" + String(durations[i]);
    }
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
}


/*
This is google app script code which we paste in google app script

function doGet(e) { 
  Logger.log(JSON.stringify(e));
  var result = 'Ok';
  
  if (Object.keys(e.parameter).length === 0) {
    result = 'No Parameters';
  } else {
    var sheet_id = '_________________________'; // Spreadsheet ID
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1; 
    var rowData = [];
    var Curr_Date = new Date();
    rowData[0] = Curr_Date; // Date in column A
    var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Kolkata", 'HH:mm:ss');
    rowData[1] = Curr_Time; // Time in column B
    
    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);
      
      switch (param) {
        case 'user_id':
          rowData[2] = value; // User ID in column C
          result = 'User ID Written in column C'; 
          break;
        case 'examiner_id':
          rowData[3] = value; // Examiner ID in column D
          result += ' , Examiner ID Written in column D'; 
          break; 
        default:
          // Check for e2, d2, ..., e10, d10
          if (param.startsWith('e') || param.startsWith('d')) {
            var index = parseInt(param.substring(1));
            if (!isNaN(index) && index > 0 && index <= 10) {
              if (param.startsWith('e')) {
                rowData[4 + (index - 1) * 2] = value; // e1, e2, ..., e10 in columns E, G, I, ...
                result += ` , ${param} Written in column ${String.fromCharCode(69 + (index - 1) * 2)}`;
              } else {
                rowData[5 + (index - 1) * 2] = value; // d1, d2, ..., d10 in columns F, H, J, ...
                result += ` , ${param} Written in column ${String.fromCharCode(70 + (index - 1) * 2)}`;
              }
            } else {
              result = "unsupported parameter";
            }
          } else {
            result = "unsupported parameter";
          }
      }
    }
    Logger.log(JSON.stringify(rowData));
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }
  
  return ContentService.createTextOutput(result);
}

function stripQuotes(value) {
  return value.replace(/^["']|['"]$/g, "");
}










*/



