#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char a[] = "NISHANT";

int p_meter = A0;

void printinlcd(int size,int curr,int place){
  char temp[] = "                ";
  for(int i=0; i<=size;i++){
      temp[i] = a[i];
  }
    temp[place] = a[curr];
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(temp);

}

void setup() {
   lcd.begin(16, 2);
   pinMode(p_meter,INPUT);
   lcd.print("nihjh");
   delay(2000);
  
}

void loop() {

// int p_value = analogRead(p_meter);
//   delay(100);
//   lcd.clear();
//   float vol = (((float)p_value)/(860.0) )* 5;
//   lcd.setCursor(0, 0);
//   lcd.print("Voltage: ");
//   lcd.print(vol);




// alphabet printing
// for(int i=65; i<102;i++){
//   char a = i;
//   delay(500);
//   lcd.clear();
//   lcd.setCursor(2, 0);
//   lcd.print(a);

//   a = i + 32;

//   lcd.setCursor(5, 0);
//   lcd.print(a);
  
// }


// pattern 4
for(int i=0; i<7; i++){
  for(int j=16;j>i;j--){
    delay(100);
    printinlcd(i,i+1,j);
  }
}



// pattern 3

// for(int i=0;i<10;i++){
//   for(int j=17;j>=i;j--){
//     lcd.clear();
//     lcd.setCursor(j, 0);
//     lcd.print(a[i]);
//   }

//   delay(50);
// }
//   lcd.clear();
//   lcd.print(a);
//   delay(200);


// pattern 2
// for(int i=0;i<17;i++){
//   lcd.clear();
//   int lst = i-9;
//   if(lst){
//     lcd.setCursor(0, 0);
//     lcd.print(a);
//   }

//   lcd.setCursor(i,0);
//     lcd.print(a);
//   delay(500);
// }

// pattern1
// for(int j=0;j<2;j++){
// for(int i = 0;i<17;i++){
//   lcd.clear();
//   lcd.setCursor(i,j);
//   lcd.print("Nishant");
//   delay(100);
// }}



}
