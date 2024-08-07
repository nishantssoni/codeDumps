
int up = 8;
int down = 9;
void setup() {
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  digitalWrite(down, LOW);
  digitalWrite(up, LOW);
}

void loop() {

  

  delay(1000);
  digitalWrite(down, HIGH);
  digitalWrite(up, LOW);
  delay(1000);

  digitalWrite(up, HIGH);
  digitalWrite(down, LOW);
  delay(1000);
}
