/*
author :: NISHANT SONI


variable guid::
servo_speed  :: delay of opening and closing of servo motor (example 10 for fast and 100 for slow)
motor_delay :: how many millisecond the motor should run
system_delay :: whole system delay
motor_start_delay :: how many seconds after a motor start


connection guid::
top ir  (pin 7)

middle ir (pin 6) 

bottom ir (pin 5)

servo should  (pin 3)


*/


#include <Servo.h>
#define servo_speed 10
#define motor_delay 1000
#define system_delay 2000
#define motor_start_delay 500


Servo myservo;
int irs[3] = { 5, 6, 7 };
int irs_data[3] = { 1, 1, 1 };
int curr = 0, prev = 0;
int up = 8;
int down = 9;


int giveLevel() {
  int one, two, three;
  one = irs_data[0];
  two = irs_data[1];
  three = irs_data[2];

  if ((one == 0) && (two == 0) && (three == 0)) return 1;
  else if ((one == 1) && (two == 0) && (three == 0)) return 2;
  else if ((one == 1) && (two == 1) && (three == 0)) return 3;
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

void motorRun(int a) {
  // 0 : for off
  // 1 for down
  // 2 for up

  if (a == 0) {
    digitalWrite(down, LOW);
    digitalWrite(up, LOW);
  } else if (a == 1) {
    digitalWrite(down, HIGH);
    digitalWrite(up, LOW);
  } else if (a == 2) {
    digitalWrite(up, HIGH);
    digitalWrite(down, LOW);
  }
  delay(300);
}

void motorUp(int num) {
  delay(motor_start_delay);
  while (num--) {
    motorRun(2);
    delay(motor_delay);
    motorRun(0);
  }
}

void motorDown(int num) {
  delay(motor_start_delay);
  while (num--) {
    motorRun(1);
    delay(motor_delay);
    motorRun(0);
  }
}

void servo(int ch) {
  if (ch) {
    for (int i = 0; i <= 180; i++) {
      myservo.write(i);
      delay(servo_speed);
    }
  } else {
    for (int i = 180; i >= 0; i--) {
      myservo.write(i);
      delay(servo_speed);
    }
  }
}



void setup() {
  Serial.begin(9600);
  myservo.attach(3);
  myservo.write(0);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);

  for (int i = 0; i < 3; i++) {
    pinMode(irs[i], INPUT);
  }
}

void loop() {
  delay(system_delay);

  curr = readData();

  if (curr > prev) {
    motorUp(curr - prev);
    if (prev == 0) {
      servo(1);
    }

  } else if (curr < prev) {
    if (curr != 0) {
      curr = 0;
    }
    servo(0);
    motorDown(prev);
  }

  prev = curr;
  Serial.println(curr);
}
