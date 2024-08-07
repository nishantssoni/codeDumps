/*
author : Nishant Soni
finding  distance using ultrasonic sensor and for blid personi and
alerting when there is object in certain distance x.
*/


#define trigPin 10
#define echoPin 13

float duration, distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration /2) * 0.0343;

  Serial.print("Distance is :: ");
    if(distance >= 400 || distance  <=2){
      Serial.println("out of range");
    }else{
      Serial.print(distance);
    }
    delay(500);


}
