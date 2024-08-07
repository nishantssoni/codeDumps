int first[] = { 2, 3, 4 };
int second[] = { 5, 6, 7 };
int mode = 0;
int firstIn = 8;
int secondIn = 9;

//ir sensor
int ir_1 = A0;
int ir_2 = A1;



int check_signal(){
  int btn_1 = digitalRead(firstIn);
  int btn_2 = digitalRead(secondIn);
  int ir1 = analogRead(ir_1);
  int ir2 = analogRead(ir_2);
  
  if((btn_1 == 1) || (btn_2 == 1) ){
    return 1;
  }
  else if((ir1 < 150) || (ir2 > 150)){
    return 1;
  }

  return 0;
}


void off() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(first[i], LOW);
    digitalWrite(second[i], LOW);
  }
}

void fun(int mode) {

  if (mode == 0) {
    off();

    for (int i = 0; i < 3; i++) {
      if(check_signal()) return;

      digitalWrite(first[i], HIGH);
      digitalWrite(second[2 - i], HIGH);
      delay(500);

      digitalWrite(first[i], LOW);
      digitalWrite(second[2 - i], LOW);
    }
    mode = 1;
  }
  if (mode == 1) {
    off();
    for (int i = 0; i < 3; i++) {
      if(check_signal()) return;

      digitalWrite(first[2 - i], HIGH);
      digitalWrite(second[i], HIGH);
      delay(500);

      digitalWrite(first[2 - i], LOW);
      digitalWrite(second[i], LOW);
    }
    mode = 0;
  }

  if (mode == 3) {
    off();
    digitalWrite(first[0], HIGH);
    digitalWrite(second[2], HIGH);
  }

  if (mode == 4) {
    off();
    digitalWrite(first[2], HIGH);
    digitalWrite(second[0], HIGH);
  }
}

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(first[i], OUTPUT);
    pinMode(second[i], OUTPUT);
  }
  pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);

  pinMode(firstIn, INPUT);
  pinMode(secondIn, INPUT);

  Serial.begin(9600);
}

void loop() {
  int val1, val2;
  int ir_val1, ir_val2;

  val1 = digitalRead(firstIn);
  val2 = digitalRead(secondIn);
  ir_val1 = analogRead(ir_1);
  ir_val2 = analogRead(ir_2);

  Serial.print(ir_val1);
  Serial.print(" ");
  Serial.print(ir_val2);
  Serial.println();

  if (val1 == 1) {
    fun(3);
    delay(500);
  } else if (val2 == 1) {
    fun(4);
    delay(500);
  } else if (ir_val1 < 150) {

    fun(3);
    delay(500);
  } else if (ir_val2 > 150) {
    fun(4);
    delay(500);

  } else {
    fun(mode);
  }
}