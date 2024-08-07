// Track 1: Dispense
// Track 2: Connected
// Track 3: WiFi Waiting
// Track 4: Success

int speaker = D3; // Pin connected to the speaker
int pus_btn = D4; // Pin connected to the push button
int prev_btn_state = HIGH; // Previous state of the button (initially HIGH)
int curr_btn_state; // Current state of the button

void setup() {
  pinMode(speaker, OUTPUT); // Set the speaker pin as output
  pinMode(pus_btn, INPUT_PULLUP); // Set the push button pin as input with internal pull-up resistor
}

void loop() {
  curr_btn_state = digitalRead(pus_btn); // Read the current state of the button

  // Check if the button was pressed (transition from HIGH to LOW)
  if ((prev_btn_state == HIGH) && (curr_btn_state == LOW)) {
    track4(); // Play the success track
  }

  prev_btn_state = curr_btn_state; // Update the previous state to the current state
}

// Function to play a tone on the speaker
// pin: Pin connected to the speaker
// duration: Duration to play the tone
// after_duration: Duration to wait after playing the tone
void playTone(int pin, int duration, int after_duration) {
  digitalWrite(pin, HIGH); // Turn the speaker on
  delay(duration); // Wait for the specified duration
  digitalWrite(pin, LOW); // Turn the speaker off
  delay(after_duration); // Wait for the specified after_duration
}

// Function to play track 1 (dispense)
void track1() {
  playTone(speaker, 20, 10);
  playTone(speaker, 40, 30);
  playTone(speaker, 40, 30);
  playTone(speaker, 20, 10);
}

// Function to play track 2 (connected)
void track2() {
  playTone(speaker, 20, 100);
  playTone(speaker, 40, 10);
  playTone(speaker, 40, 10);
  playTone(speaker, 20, 100);
}

// Function to play track 3 (WiFi waiting)
void track3() {
  playTone(speaker, 30, 10);
  playTone(speaker, 10, 100);
  playTone(speaker, 30, 10);
}

// Function to play track 4 (success)
void track4() {
  playTone(speaker, 500, 100);
  playTone(speaker, 5, 5);
  playTone(speaker, 30, 50);
  playTone(speaker, 20, 20);
  playTone(speaker, 20, 20);
}
