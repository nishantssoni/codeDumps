#include "BluetoothSerial.h"

BluetoothSerial bt;
String Data;

int result;


void setup() {
  Serial.begin(115200);
  bt.begin("nisant-bt");
  pinMode(2, OUTPUT);
}



int test() {
  delay(5000);
  return 32;
}

void loop() {


  if (bt.available()) {

    while (bt.available()) {
      char c = bt.read();
      Data += c;
    }

    if (Data == "start") {
      result = test();

      Serial.print(" the test is finished your result :: ");

      delay(1000);

      bt.print(result);
      Serial.print("your result is send to the app ");
    }



    Data = "";
    result = 0;

    delay(20);
  }

}
