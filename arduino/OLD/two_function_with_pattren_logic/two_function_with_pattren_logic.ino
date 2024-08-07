int btn1 = 2;
int btn2 = 3;
int btn3 = 4;

int ptt[20];
int index = -1;

void setup() {
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  Serial.begin(9600);
}

void reset() {
  index = -1;
  Serial.println("pttrn reset");
}

void print() {
  if (index == -1) return;
  for (int i = 0; i <= index; i++) {
    Serial.print(ptt[i]);
  }
  Serial.println();
}

void loop() {
  int ch1 = digitalRead(btn3);
  int ch2 = digitalRead(btn2);
  int ch3 = digitalRead(btn1);



  if (ch1 == 1) {
    ptt[++index] = 1;
  }
  while (digitalRead(btn3) == 1)
    ;

  if (ch2 == 1) {
    ptt[++index] = 0;
  }
  while (digitalRead(btn2) == 1)
    ;

  if (ch3 == 1) {
    print();

    delay(500);
    reset();
  }
  while (digitalRead(btn1) == 1)
    ;

  delay(200);
}
