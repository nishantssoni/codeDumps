/*
10 - level 3
9 - level 2
8 - level 1

*/
#include<Servo.h>

int irs[3] = { 8, 9, 10 };
int irs_data[3] = {1,1,1};
Servo myservo;


int up1 = 2;
int down1 = 3;
int down = 5;
int up = 4;

int curr=0, prev=0,state=0;

int startDetecting(){
  int one, two, three;
  one = irs_data[0];
  two = irs_data[1];
  three = irs_data[2];

  if ((one == 0) || (two == 0) || (three == 0)) return 1;

  return 0;
}

int giveLevel() {
  int one, two, three;
  one = irs_data[0];
  two = irs_data[1];
  three = irs_data[2];

  if ((one == 1) && (two == 1) && (three == 0)) return 3;
  else if ((one == 1) && (two == 0) && (three == 0)) return 2;
  else if ((one == 0) && (two == 0) && (three == 0)) return 1;
  return 0;
}

int readData() {
  int level;
  for (int i = 0; i < 3; i++) {
      irs_data[i] = digitalRead(irs[i]);
  }
  level = giveLevel();
  return level;
}

void driveMotorUp(int a,int b){

  for(int i = 1; i<=(a-b);i++){
    digitalWrite(up,HIGH);
    digitalWrite(up1,HIGH);

    delay(1000);
    digitalWrite(up,LOW);
    digitalWrite(up1,LOW);
    delay(1000);
  }
  myservo.write(90);

}
void driveMotorDown(int a){
  myservo.write(-90);

  for(int i = a; i>0;i--){
    digitalWrite(down,HIGH);
    digitalWrite(down1,HIGH);
    delay(1000);
    digitalWrite(down,LOW);
    digitalWrite(down1,LOW);
    delay(1000);
  }

}

void print(){
  Serial.print(irs_data[0]);
  Serial.print(" ");
  Serial.print(irs_data[1]);
  Serial.print(" ");
  Serial.print(irs_data[2]);
  Serial.print("    ");

  Serial.print(giveLevel());

  Serial.println();

}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(irs[i], INPUT);
  }
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(up1, OUTPUT);
  pinMode(down1, OUTPUT);
  myservo.attach(11);

}

void loop() {

  delay(3000);
  curr = readData();
  if(curr>prev) driveMotorUp(curr,prev);
  else if(prev>curr) driveMotorDown(prev);
  prev = curr;

  // readData();
  // print();
  // if(startDetecting() && (state == 0)){
  //   state = 1;
  // }
  

  // if(state)
  // {
  //   delay(2000);
  //   print();
  //   curr = readData();
  //   if(curr>prev) driveMotorUp(curr,prev);
  //   else if(prev>curr) driveMotorDown(prev);
  //   print();
   
  //   Serial.println(prev);
  //   Serial.print("  ");
  //   Serial.print(curr);
  //   Serial.println();
  //    prev = curr;
  //   if((prev == 0) && (curr ==0)) state = 0;

  // }

}
