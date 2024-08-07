int trg = 8;
int eho = 9;
int curr = 0;

int zones[] = { 2, 3, 4, 5, 6 };

int duration = 0;
int distance = 0;

void off(int a) {
  for (int i = 0; i < 5; i++) {
    if (i == curr) continue;
    digitalWrite(zones[i], LOW);
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(trg, OUTPUT);
  pinMode(eho, INPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(zones[i], OUTPUT);
  }
}
void loop() {
  digitalWrite(trg, LOW);
  delay(2);
  digitalWrite(trg, HIGH);
  delay(10);
  digitalWrite(trg, LOW);

  duration = pulseIn(eho, HIGH);

  distance = (duration  / 2)* 0.0343;
  Serial.println(distance);

  if (distance > 80) {
    digitalWrite(zones[0], HIGH);
    curr = 0;
  } else if (distance > 60 && distance < 80) {
    digitalWrite(zones[1], HIGH);
    curr = 1;
  } else if (distance > 40 && distance < 60) {
    digitalWrite(zones[2], HIGH);
    curr = 2;
  } else if (distance > 20 && distance < 40) {
    digitalWrite(zones[3], HIGH);
    curr = 3;
  } else if (distance < 20) {
    digitalWrite(zones[4], HIGH);
    curr = 4;
  }
  off(curr);
}
