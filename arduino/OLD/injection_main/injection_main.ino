#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_Keypad.h"

#define m1 10
#define m2 11
#define buzzer 12

// define your specific keypad here via PID
#define KEYPAD_PID3844
// define your pins here
#define ir 10


// can ignore ones that don't apply
#define R1 9
#define R2 8
#define R3 7
#define R4 6
#define C1 5
#define C2 4
#define C3 3
#define C4 2
// leave this import after the above configuration
#include "keypad_config.h"

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char currDose = 'A';

void iniit() {

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Infusion System!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initialising!");
  delay(1000);
  lcd.setCursor(0, 2);
  lcd.setCursor(0, 0);
  lcd.print("Power Robotics Lab!");
  delay(1000);
  lcd.clear();
}
void currDoseStr(){
  lcd.clear();
  lcd.print("A:50 B:100 C:rst");
  lcd.setCursor(0, 1);
  lcd.print("current :");
  lcd.print(currDose);
}

void setup() {
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(ir, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  customKeypad.begin();
  iniit();
  currDoseStr();
  reset();
}

char keyPress() {
  char key;
  customKeypad.tick();


  while (customKeypad.available()) {
    keypadEvent e = customKeypad.read();
    key = (char)e.bit.KEY;
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      return key;
    };
    // else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }
  return 'f';
}

void off() {
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
}
void run(int a){
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
  delay(a);
  off();
}

void reset(){
  lcd.clear();
  lcd.print("Resetting...");
  if(!digitalRead(ir)){
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
  }
  while(digitalRead(ir));
  off();
  lcd.clear();

  Serial.println("system reset");
  
}

void buzz(int a){
  digitalWrite(buzzer, HIGH);
  delay(a);
  digitalWrite(buzzer, LOW);
}

void inject(char key) {
  lcd.clear();
  if (key == 'A') {
    lcd.print("Dosage:50 ml");
    run(5000);
    lcd.clear();
    lcd.print("Dosage Completed");
    buzz(3000);
    lcd.clear();
    
  } else if (key == 'B') {
    
    lcd.print("Dosage:100 ml");
    run(10000);
    lcd.clear();
    lcd.print("Dosage Completed");
    buzz(3000);
    lcd.clear();
  }


  if (key == 'C') {
    lcd.print("Rst infusion pump");
    reset();
    lcd.clear();
    lcd.print("Restarting the systm");
    buzz(3000);
    lcd.clear();
}
currDoseStr();
}

void loop() {
  
  
  char k = 'f';
  k = keyPress();


  if (k != 'f') {

    if(k != 'D'){
      currDose = k;
      currDoseStr();
    }else{
      inject(currDose);
    }

  delay(500);
    
  }


}