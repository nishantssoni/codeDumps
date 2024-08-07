#include <LiquidCrystal_I2C.h>

// enter the I2C address and the dimensions of your LCD here
LiquidCrystal_I2C lcd(0x27, 16, 2);

int place=7;
int change = 6;

int num[3]={0};
int curr_index = 1;

void setup() {
  Serial.begin(9600);
  
  pinMode(place,INPUT_PULLUP);
  pinMode(change,INPUT_PULLUP);
  


  
  lcd.begin();
  lcd.clear();         
  lcd.backlight();
  
  // // Print a message on both lines of the LCD.
  // lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  // lcd.print("Hello world!");
  
  // lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  // lcd.print("LCD Tutorial");
}

void loop() {
    int ch = digitalRead(place);
    int pl = digitalRead(change);

    

    if(ch==0){
      if(num[curr_index] > 8) num[curr_index] = -1;
      num[curr_index]++;

    }while(digitalRead(place)==0);
    if(pl==0){
      curr_index++;
      if(curr_index>2) curr_index = 0;
    }while(digitalRead(change)==0);

    lcd.clear();
    lcd.setCursor(2,0); 
    lcd.print(num[2]);
    lcd.print(num[1]);
    lcd.print(num[0]);

    lcd.setCursor(2,1);
    if(curr_index == 0){
      lcd.print("  ^");
    }else if(curr_index == 1){
      lcd.print(" ^ ");
    }else if(curr_index == 2){
      lcd.print("^  ");
    }

    delay(100);



}