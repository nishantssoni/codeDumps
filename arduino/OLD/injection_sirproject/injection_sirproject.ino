#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#include "Adafruit_Keypad.h"

// define your specific keypad here via PID
#define KEYPAD_PID3844
// define your pins here
// can ignore ones that don't apply
#define R1    2
#define R2    3
#define R3    4
#define R4    5
#define C1    8
#define C2    9
#define C3    10
#define C4    11
// leave this import after the above configuration
#include "keypad_config.h"

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27,16,2);

int isOk = 1;

void setup() {
  Serial.begin(9600);
  customKeypad.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Infusion System!");

}

void loop() {
  
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    char key = (char)e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED && (isOk)){
      lcd.print(key);
      delay(1000);
    };
    // else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
    
  }

}
