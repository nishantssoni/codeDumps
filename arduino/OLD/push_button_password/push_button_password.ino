#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

int red = 8;
int green = 7;
int button1 = 2;
int button2 = 3;
int button3 = 4;
int button4 = 5;
char buttonState = 0;

void setup() {
pinMode(button1,INPUT);
pinMode(button2,INPUT);
pinMode(button3,INPUT);
pinMode(button4,INPUT);

pinMode(red,OUTPUT);
pinMode(green,OUTPUT);

lcd.begin();

}
 
void loop() {

  buttonState = digitalRead(button1),(button2),(button3),(button4);
  lcd.println(buttonState);

  if (buttonState == HIGH) {
digitalWrite(red, HIGH); 
delay(2000);
lcd.clear();
lcd.print("Led On");
delay(2000);
lcd.clear();

}

else {

digitalWrite(red, LOW);

  
}
}