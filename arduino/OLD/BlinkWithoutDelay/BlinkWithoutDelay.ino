
const int din =  2;
const int dout = 3;

int ir_state_in, ir_state_out;
int counter = 0;
int stand_in = 0;
int stand_out = 0;

void setup() {
  pinMode(din, INPUT);
  pinMode(dout, INPUT);
  Serial.begin(9600);
}

void loop() {
 ir_state_in = digitalRead(din);
 ir_state_out = digitalRead(dout);

 if(!ir_state_in){
   stand_in = 0;
 }
  if(!ir_state_out){
   stand_out = 0;
 }

  if((ir_state_in) && (stand_in == 0)){
    stand_in = 1;
    counter++;
    
    delay(500);
    Serial.print("total person in room are :: ");
    Serial.println(counter);

  }
  if((ir_state_out) && (stand_out == 0)){
    stand_out = 1;
    if(counter > 0) counter--;
    delay(500);
    Serial.print("total person in room are :: ");
    Serial.println(counter);

  }

}
