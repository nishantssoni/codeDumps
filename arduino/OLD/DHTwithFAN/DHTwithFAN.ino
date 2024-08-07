#include <DHT.h>
#include <LedControl.h>
#define DHTPIN 3
#define DHTTYPE DHT11


int DIN = 12;
int CS = 11;
int CLK = 10;
LedControl lc = LedControl(DIN, CLK, CS, 0);



DHT dht(DHTPIN, DHTTYPE);

int de = 500;


byte fan[2][8] = { { 0b00011000,
                     0b00011000,
                     0b00011000,
                     0b11111111,
                     0b11111111,
                     0b00011000,
                     0b00011000,
                     0b00011000 },
                   { 0b11000011,
                     0b11100111,
                     0b01100110,
                     0b00011000,
                     0b00011000,
                     0b01100110,
                     0b11100111,
                     0b11000011 } };




void setup() {

  Serial.begin(9600);
  Serial.println("Humidity and tempreature sensor");
  dht.begin();
  lc.shutdown(0, false);
  lc.setIntensity(0, 10);
  lc.clearDisplay(0);
}

void loop() {

  printByte(fan[0]);
  delay(de);
  printByte(fan[1]);
  delay(de);
  printByte(fan[0]);

  if (dht.read()) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    Serial.print(" HUMIDITY :: ");
    Serial.print(h);
    Serial.println(" % RH");

    Serial.print(" TEMPREATURE :: ");
    Serial.print(t);
    Serial.print(" *C\n\n\n");

    int mappedVal = map(t, 0, 40, 2000, 0);
    de = mappedVal;

    Serial.print("\nFan speed :: ");
    Serial.println(de);
  }
}

void printByte(byte character[]) {
  int i = 0;
  for (i = 0; i < 8; i++) {
    lc.setRow(0, i, character[i]);
  }
}
