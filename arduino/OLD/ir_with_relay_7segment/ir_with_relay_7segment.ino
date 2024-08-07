/*
code: person counter which will display in 7 segment display 
      and relay turned on when person count is above 5.
*/

int led[] = { 2, 3, 4, 5, 6, 7, 8, 9 };  //seven segment display pin
int ir_1 = A0;                           //ir sensor
int relay = 10;                          //relay
int counter = 0;                         // counter which count person
int ir_state, prev_state = 0;

// num[] store pattern of all digits from 0-9
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


// check_signal() check if a person is front of ir sensor and return 1 if true
//return 0 if false
int check_signal() {
  int ir1 = analogRead(ir_1);
  if (ir1 > 150) {
    return 1;
  }
  return 0;
}

// place() put digit in 7 segment display from patter which is stored in num[]
void place(char a[]) {
  for (int i = 0; i < 8; i++) {
    if (a[i] == '1') {
      digitalWrite(led[i], HIGH);
    }
  }
}

// off() off all the leds in 7 segment display
void off() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(led[i], LOW);
  }
}


void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(led[i], OUTPUT);
  }
  pinMode(ir_1, INPUT);
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  off();

  place(num[counter]);  //initiate with 0 in display
}

void loop() {
  ir_state = check_signal();

  // prev_state keep track of the fact that a person is standing
  // in frot of sensor will only counted by 1.

  if (ir_state && (prev_state == 0)) {
    counter++;
    prev_state = 1;
    off();
    place(num[counter]);
  } else if (ir_state == 0) {
    prev_state = 0;
  }
  if (counter > 5) {
    digitalWrite(relay, HIGH);
  }

  delay(50);
}
