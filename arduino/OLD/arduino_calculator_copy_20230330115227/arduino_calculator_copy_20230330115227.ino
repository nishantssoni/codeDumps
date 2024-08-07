int num1, num2, choice = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) Serial.read();

  Serial.println("\nenter the first number :: ");
  while (Serial.available() == 0) {}
  num1 = Serial.parseInt();

  while (Serial.available() > 0) Serial.read();

  Serial.println("\nenter the second number :: ");
  while (Serial.available() == 0) {}
  num2 = Serial.parseInt();

  while (Serial.available() > 0) Serial.read();

  // Print well formatted output
  Serial.print("Your choice are \n1.addition\n2.subtraction\n3.multiplication\n4.division\n");
  while (Serial.available() == 0) {}
  choice = Serial.parseInt();
  while (Serial.available() > 0) Serial.read();

  if (choice == 0) {
    Serial.println("enter a choice in int format something went wrong !!\n\n");

  } else if (choice == 1) {
    Serial.print(" addition of two number is :: ");
    Serial.print(num1 + num2);
  } else if (choice == 2) {
    Serial.print(" subtraction of two number is :: ");
    Serial.print(num1 - num2);
  } else if (choice == 3) {
    Serial.print(" multiplication of two number is :: ");
    Serial.print(num1 * num2);
  } else if (choice == 4) {
    Serial.print(" division of two number is :: ");
    Serial.print(float(num1 / num2));
  }

  Serial.print("\n\n\n");
  delay(100);
}
