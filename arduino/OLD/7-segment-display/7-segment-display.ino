int led[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
int light[] = { 10, 11, 12 };
int num[] = {
  "01111110",
  "00010010",
  "10111100",
  "10110110",
  "11010010",
  "11100110",
  "11101110",
  "00110010",
  "11111110",
  "11110110"
};

void place(char a[]) {
  for (int i = 0; i < 8; i++) {
    if (a[i] == '1') {
      digitalWrite(led[i], HIGH);
    }
  }
}


void off() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(led[i], LOW);
  }
}


void traffic(int a) {

  digitalWrite(light[0], LOW);
  digitalWrite(light[2], LOW);
  digitalWrite(light[1], HIGH);
  delay(1000);
  digitalWrite(light[1], LOW);

  if (a == 1) {
    digitalWrite(light[0], HIGH);
  } else if (a == 2) {
    digitalWrite(light[2], HIGH);
  }
}


void setup() {

  for (int i = 0; i < 8; i++) {
    pinMode(led[i], OUTPUT);
  }

  for (int i = 0; i < 3; i++) {
    pinMode(light[i], OUTPUT);
  }
}

void loop() {
  for (int j = 1; j <= 2; j++) {
    traffic(j);
    for (int i = 0; i < 10; i++) {
      place(num[9 - i]);
      delay(500);
      off();
    }
  }
}