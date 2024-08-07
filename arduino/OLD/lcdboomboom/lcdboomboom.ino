#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal LCD(rs, en, d4, d5, d6, d7);
int ir = 2;
int counter = 0;
int state = 0;
int led = 8; 

void setup() {
  LCD.begin(16, 2);
  Serial.begin(9600);
  pinMode(ir, INPUT);
  pinMode(led,OUTPUT);
  LCD.print("START");
  
}

void loop() {
  int dir = digitalRead(ir);
  if(dir == 1) state = 0;

  if((dir == 0) && (state == 0)){
    counter++;
    state = 1;
    LCD.clear();

    if(counter <= 9){
    LCD.print("Goal Count ");
    LCD.print(counter);
    }
    if(counter == 9){
      delay(500);
      LCD.clear();
      LCD.print("WELCOME TO HELL");
    }
    if(counter > 9){
      delay(500);
      LCD.clear();
      LCD.print("BOOM BOOM");
      LCD.setCursor(0, 1);
      LCD.print("YOU ARE DEAD::::");
      digitalWrite(led,HIGH);
    }

    // resetting feature

    // if(counter == 15){
    //   counter = 0;
    //   LCD.clear();
    //   digitalWrite(led,LOW);
    //   LCD.print("RESETTING....");
    //   delay(2000);
    //   LCD.clear();
    //   LCD.print("START");
    // }
  
  }
  delay(100);

}
