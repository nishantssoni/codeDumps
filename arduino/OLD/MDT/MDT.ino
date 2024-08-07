#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int finish = D5;
const int star = D3;

const int led1 = D7;
const int led2 = D6;

int count = 0;  
int starstate = LOW;
int finishstate = LOW;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void printLCD(int a){
    lcd.setCursor(0, 0);            
    lcd.print("Count");   
    lcd.setCursor(6, 0);
    lcd.print(count);
    lcd.setCursor(0, 1);      
    lcd.print("MM PsiQ (WiFi)"); 
}

void setup() {
  pinMode(finish, INPUT_PULLUP);
  pinMode(star, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  Serial.begin(115200);              // Initialize serial communication
  lcd.begin();                      // Initialize the LCD display.
  lcd.backlight(); 
  printLCD(0);
}

void loop() {
  starstate = digitalRead(star);
  finishstate = digitalRead(finish);

  if (starstate == LOW) {
    count++;
    printLCD(count);
  }while(digitalRead(star) == LOW);
  
  delay(100);

  if (finishstate == LOW) {
    digitalWrite(led2,HIGH);
    delay(300);
    digitalWrite(led2,LOW);
  }while(digitalRead(finish) == LOW);

  

}
