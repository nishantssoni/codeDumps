int buzz = 12;
int leds[] = {11,2,3};
int btn[] = {8,9,10};

int reset = 0;

void off(){
  for(int i = 0; i<3; i++){
      digitalWrite(leds[i],LOW);
    }
}

void buzzz(){
  digitalWrite(buzz,HIGH);
  delay(500);
  digitalWrite(buzz,LOW);
}


void setup() {
  pinMode(buzz,OUTPUT);

  for(int i =0;i<3; i++){
    pinMode(leds[i],OUTPUT);
    pinMode(btn[i],INPUT);
  }
  off();
}

void loop() {
  int a,b,c;
  a = digitalRead(btn[0]);
  b = digitalRead(btn[1]);
  c = digitalRead(btn[2]);

  if(a && b && c){
    off();
    delay(1000);
    a=b=c=0;
    reset = 0;
  }

  if(!reset){

    if(a == 1){
      digitalWrite(leds[0],HIGH);
      buzzz();
      reset = 1;
    }
    else if(b == 1){
      digitalWrite(leds[1],HIGH);
      buzzz();
      reset = 1;
    }
    else if(c == 1){
      digitalWrite(leds[2],HIGH);
      buzzz();
      reset = 1;
    }
  }
}