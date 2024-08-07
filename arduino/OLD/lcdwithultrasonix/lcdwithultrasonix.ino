#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
int trg = 10;
int eho = 11;
int fan = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float chekDis(){
  digitalWrite(trg, LOW);
  delay(2);
  digitalWrite(trg, HIGH);
  delay(10);
  digitalWrite(trg, LOW);

  int duration = pulseIn(eho, HIGH);

  float distance = duration * 0.034 / 2;

  return distance;
}

void setup() {
  lcd.begin(16, 2);
  pinMode(trg, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(eho, INPUT);

}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("dis(CM):");
  lcd.print(chekDis());

  lcd.setCursor(0, 1);
  lcd.print("dis(INC):");
  lcd.print((chekDis()/2.54));
  delay(200);
  if(chekDis() < 10){
      digitalWrite(fan,HIGH);
      
  }else{
    digitalWrite(fan,LOW);
  }

}
