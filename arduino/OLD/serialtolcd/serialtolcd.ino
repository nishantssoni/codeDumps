/*
code: timer with lcd
author : nishant soni

*/
#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int ti = 100;
int data = 0;
int btn = 10;

void wait(int a) {
  float time = 0;
  for (int i = 1; i <= a; i++) {
    delay(1);
    if (i % 500 == 0) {
      time += 0.5;
      lcd.clear();
      lcd.print(time);
    }
  }
}

void setup() {
  pinMode(btn, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  int bstate = digitalRead(btn);

  while (Serial.available() > 0) {
    int data = (int)Serial.parseInt();
    if (data > 1000) {
      ti = data;
      lcd.clear();
      lcd.print("timer in ms: ");
      lcd.setCursor(0, 1);
      lcd.print(ti);
      delay(2000);
      lcd.clear();
      lcd.print("push btn to start");
    }
  }
  if (bstate == 1) {
    lcd.clear();
    wait(ti);
    lcd.clear();
    lcd.print("TIME END !!");
  }
}