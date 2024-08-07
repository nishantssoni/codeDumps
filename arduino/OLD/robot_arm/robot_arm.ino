#include<Servo.h>
Servo myservo;
Servo myservo1;

int wrist[2] = {6,7};
int arm[2] = {8,9};

int w_val = 0;
int a_val = 0;

void setup() {

  Serial.begin(9600);
  myservo.attach(12);
  myservo1.attach(11);
  pinMode(wrist[0],INPUT);
  pinMode(wrist[1],INPUT);
  pinMode(arm[0],INPUT);
  pinMode(arm[1],INPUT);

}

void loop(){
  int w_up = digitalRead(wrist[0]);
  int w_dn = digitalRead(wrist[1]);
  int a_up = digitalRead(arm[0]);
  int a_dn = digitalRead(arm[1]);
 
 if(w_up == 1){
   if(w_val < 180){
     w_val += 1;
   }
 }
 else if (w_dn == 1) {
   if(w_val > 0){
     w_val -= 1;
   }
 }

  if(a_up == 1){
   if(a_val < 180){
     a_val += 1;
   }
 }
 else if (a_dn == 1) {
   if(a_val > 0){
     a_val -= 1;
   }
 }


 
 
 myservo.write(a_val);
 myservo1.write(w_val);
 delay(10);
 Serial.print(a_val);
 Serial.print("  ");
 Serial.println(w_val);

}
