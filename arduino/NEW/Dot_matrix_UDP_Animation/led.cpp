#include "led.h"
#include <MD_MAX72xx.h>
#include <SPI.h>



void setupLED() {
  // MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

  matrix.begin();
  matrix.clear();
}

int format_string_anim(String req) {
  String numberPart = req.substring(7);  // Get the substring after "slider_"
  int sliderValue = numberPart.toInt();
  // Serial.print("animate Value: ");
  // Serial.println(sliderValue);
  return sliderValue;
}

int format_string(String req) {
  String numberPart = req.substring(7);  // Get the substring after "slider_"
  float sliderValueFloat = numberPart.toFloat();
  int sliderValue = round(sliderValueFloat);
  // Serial.print("Slider Value: ");
  // Serial.println(sliderValue);
  return sliderValue;
}

// Function to set brightness (0 to 15)
void setBrightness(int sliderValue) {
  // Map the slider value (0-100) to brightness (0-15)
  uint8_t brightness = map(sliderValue, 0, 100, 0, 15);
  matrix.control(MD_MAX72XX::INTENSITY, brightness);
}
void setSpeed(int a) {
  a = 100 - a;
  speed_val = a * 3;
}

void diagonalLine() {
  static int i = 0;  // Keep track of the current step
  static unsigned long previousMillis = 0;
  const long interval = speed_val;  // Time between updates (in milliseconds)

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();  // Clear the display
    for (int j = 0; j <= i; j++) {
      matrix.setPoint(j, j, true);  // Light up the diagonal
    }

    i++;  // Move to the next step
    if (i >= 8) {
      i = 0;  // Reset after completing the diagonal
    }
  }
}

void bouncingBall() {
  static int i = 0;
  static bool goingDown = true;  // Track whether the ball is going down or up
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();               // Clear the display
    matrix.setPoint(i, i, true);  // Set the current position of the ball

    // Update the position of the ball
    if (goingDown) {
      i++;
      if (i >= 7) goingDown = false;  // Reverse direction when reaching the bottom
    } else {
      i--;
      if (i <= 0) goingDown = true;  // Reverse direction when reaching the top
    }
  }
}


void blinkCorners() {
  static bool state = false;  // Track whether the corners are on or off
  static unsigned long previousMillis = 0;
  const long interval = speed_val;  // Blink interval (500 ms)

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    if (state) {
      matrix.clear();  // Turn off the corners
    } else {
      matrix.setPoint(0, 0, true);  // Top-left corner
      matrix.setPoint(0, 7, true);  // Top-right corner
      matrix.setPoint(7, 0, true);  // Bottom-left corner
      matrix.setPoint(7, 7, true);  // Bottom-right corner
    }

    state = !state;  // Toggle state for the next blink
  }
}

void movingLine() {
  static int x = 0;
  static bool movingRight = true;  // Track whether the line is moving right or left
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();  // Clear the display
    for (int y = 0; y < 8; y++) {
      matrix.setPoint(x, y, true);  // Draw the line at the current x position
    }

    // Update the position of the line
    if (movingRight) {
      x++;
      if (x >= 7) movingRight = false;  // Reverse direction when reaching the right side
    } else {
      x--;
      if (x <= 0) movingRight = true;  // Reverse direction when reaching the left side
    }
  }
}


void zigzagPattern() {
  static int x = 0;
  static int y = 0;
  static bool movingRight = true;  // Track whether the movement is to the right or left
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();               // Clear the display
    matrix.setPoint(x, y, true);  // Set the current position of the zigzag

    // Update the position for the zigzag
    if (movingRight) {
      x++;
      if (x >= 7) {
        movingRight = false;
        y = (y + 1) % 8;  // Move to the next row when reaching the end
      }
    } else {
      x--;
      if (x <= 0) {
        movingRight = true;
        y = (y + 1) % 8;  // Move to the next row when reaching the start
      }
    }
  }
}


void flashingSquare() {
  static int size = 1;
  static bool growing = true;  // Track whether the square is growing or shrinking
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();  // Clear the display

    // Draw the square
    for (int x = 0; x < size; x++) {
      for (int y = 0; y < size; y++) {
        matrix.setPoint(x, y, true);
      }
    }

    // Update the size of the square
    if (growing) {
      size++;
      if (size > 7) growing = false;  // Start shrinking when reaching max size
    } else {
      size--;
      if (size < 1) growing = true;  // Start growing when reaching min size
    }
  }
}


void spiralPattern() {
  static int state = 0;  // State to track the spiral's progress
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();  // Clear the display

    int x = 0, y = 0, layer = 0;
    while (layer <= state) {
      for (x = layer; x < 8 - layer; x++) matrix.setPoint(x, layer, true);          // Top row
      for (y = layer + 1; y < 8 - layer; y++) matrix.setPoint(7 - layer, y, true);  // Right column
      for (x = 6 - layer; x >= layer; x--) matrix.setPoint(x, 7 - layer, true);     // Bottom row
      for (y = 6 - layer; y > layer; y--) matrix.setPoint(layer, y, true);          // Left column

      layer++;
    }

    state++;
    if (state > 3) state = 0;  // Reset when the spiral reaches the center
  }
}

void rainEffect() {
  static unsigned long previousMillis = 0;
  const long interval = speed_val;
  static byte rain[8] = { 0 };  // Store which pixels are "falling" in each column

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();  // Clear the display

    // Randomly generate new raindrops at the top
    for (int x = 0; x < 8; x++) {
      if (random(0, 10) > 7) rain[x] |= 1 << 7;  // Randomly add raindrops in each column
    }

    // Update rain state
    for (int x = 0; x < 8; x++) {
      for (int y = 7; y >= 0; y--) {
        if (rain[x] & (1 << y)) {
          matrix.setPoint(x, y, true);  // Set the raindrop on the display
          if (y > 0) {
            rain[x] &= ~(1 << y);       // Clear the current bit
            rain[x] |= (1 << (y - 1));  // Move the raindrop down
          } else {
            rain[x] &= ~(1 << y);  // Clear raindrop when it reaches the bottom
          }
        }
      }
    }
  }
}

void wavePattern() {
  static unsigned long previousMillis = 0;
  const long interval = speed_val;
  static float phase = 0.0;     // Phase of the sine wave
  const float increment = 0.3;  // Speed of the wave

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();  // Clear the display

    // Calculate and set the wave pattern
    for (int x = 0; x < 8; x++) {
      int y = 3 + round(sin(phase + x * 0.8) * 3);  // Adjust wave height and position
      matrix.setPoint(x, y, true);                  // Draw the wave point
    }

    phase += increment;                   // Move the wave forward
    if (phase > TWO_PI) phase -= TWO_PI;  // Keep phase within bounds
  }
}

void rotatingSquare() {
  static int state = 0;  // State to track the rotation steps
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    switch (state) {
      case 0:
        // Square's initial position (not rotated)
        matrix.setPoint(2, 2, true); matrix.setPoint(2, 5, true);
        matrix.setPoint(5, 2, true); matrix.setPoint(5, 5, true);
        break;
      case 1:
        // 90 degrees rotated position
        matrix.setPoint(3, 1, true); matrix.setPoint(4, 1, true);
        matrix.setPoint(3, 6, true); matrix.setPoint(4, 6, true);
        break;
      case 2:
        // 180 degrees rotated position
        matrix.setPoint(1, 3, true); matrix.setPoint(1, 4, true);
        matrix.setPoint(6, 3, true); matrix.setPoint(6, 4, true);
        break;
      case 3:
        // 270 degrees rotated position
        matrix.setPoint(2, 2, true); matrix.setPoint(2, 5, true);
        matrix.setPoint(5, 2, true); matrix.setPoint(5, 5, true);
        break;
    }

    // Increment and reset state for continuous rotation
    state++;
    if (state > 3) state = 0;
  }
}

void fireEffect() {
  static byte fire[8][8] = {0};  // Store the intensity of each pixel
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time

    matrix.clear();  // Clear the display

    // Generate new "fire" at the bottom row (with random intensity)
    for (int x = 0; x < 8; x++) {
      fire[x][7] = random(0, 4);  // Random intensity from 0 to 3 (higher fire range)
    }

    // Move the fire upwards and fade it out
    for (int y = 6; y >= 0; y--) {
      for (int x = 0; x < 8; x++) {
        // Blend intensity from the row below and fade it
        fire[x][y] = (fire[x][y] + fire[x][y + 1] + random(0, 2)) / 2;  // Fade the fire

        // Set the intensity threshold for display
        if (fire[x][y] > 1) {
          matrix.setPoint(x, y, true);  // Light up the pixel if intensity is high enough
        }
      }
    }
  }
}



void starfield() {
  static struct Star {
    int x, y;
    float z;  // Depth of the star
  } stars[10];  // Array to store 10 stars
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Update star positions
    for (int i = 0; i < 10; i++) {
      stars[i].z -= 0.1;  // Move the star closer by decreasing z
      if (stars[i].z <= 0) {  // Reset star when it's too close
        stars[i].x = random(0, 8);
        stars[i].y = random(0, 8);
        stars[i].z = random(1, 8);
      }

      // Calculate star size based on z (depth)
      int screenX = (stars[i].x / stars[i].z) * 4 + 4;
      int screenY = (stars[i].y / stars[i].z) * 4 + 4;

      if (screenX >= 0 && screenX < 8 && screenY >= 0 && screenY < 8) {
        matrix.setPoint(screenX, screenY, true);  // Draw the star
      }
    }
  }
}

void plasmaEffect() {
  static float t = 0;  // Time variable for wave movement
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Create plasma effect by calculating sine waves for each pixel
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        float value = sin(x * 0.5 + t) + sin(y * 0.5 + t);
        if (value > 0.0) matrix.setPoint(x, y, true);  // Set pixel if value is positive
      }
    }

    t += 0.05;  // Increment time for wave motion
  }
}

void fireworksEffect() {
  static struct Particle {
    float x, y;
    float dx, dy;  // Direction of movement
    int life;      // Life duration of the particle
  } particles[20];  // Store up to 20 particles
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Create new explosion randomly
    if (random(0, 100) > 95) {
      for (int i = 0; i < 20; i++) {
        particles[i].x = 4;
        particles[i].y = 4;  // Start from the center
        particles[i].dx = random(-100, 100) / 100.0;  // Random direction
        particles[i].dy = random(-100, 100) / 100.0;
        particles[i].life = random(5, 10);  // Random life duration
      }
    }

    // Update and draw particles
    for (int i = 0; i < 20; i++) {
      if (particles[i].life > 0) {
        particles[i].x += particles[i].dx;
        particles[i].y += particles[i].dy;
        particles[i].life--;

        // Set particle on the matrix
        if (particles[i].x >= 0 && particles[i].x < 8 && particles[i].y >= 0 && particles[i].y < 8) {
          matrix.setPoint((int)particles[i].x, (int)particles[i].y, true);
        }
      }
    }
  }
}

void sineWavePattern() {
  static float phase = 0.0;  // Phase of the sine wave
  static unsigned long previousMillis = 0;
  const long interval = speed_val;  // Speed of wave motion

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Generate sine wave pattern across the matrix
    for (int x = 0; x < 8; x++) {
      float value = sin(phase + x * 0.8) * 3.5 + 3.5;  // Sine wave formula, adjusted to fit 8x8 matrix
      int y = round(value);  // Round the result to get pixel positions
      matrix.setPoint(x, y, true);  // Set pixel at (x, y) to form the sine wave
    }

    phase += 0.1;  // Increment phase to move the wave
    if (phase > TWO_PI) phase -= TWO_PI;  // Reset phase after full cycle
  }
}
void cosineWavePattern() {
  static float phase = 0.0;  // Phase of the cosine wave
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Generate cosine wave across the matrix
    for (int x = 0; x < 8; x++) {
      float value = cos(phase + x * 0.8) * 3.5 + 3.5;  // Cosine wave formula
      int y = round(value);  // Round the result to fit into matrix grid
      matrix.setPoint(x, y, true);  // Set pixel at (x, y) for the cosine wave
    }

    phase += 0.1;  // Increment phase for movement
    if (phase > TWO_PI) phase -= TWO_PI;  // Reset after completing a cycle
  }
}

void dualWavePattern() {
  static float phase = 0.0;
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Generate both sine and cosine waves
    for (int x = 0; x < 8; x++) {
      // Sine wave
      float sineValue = sin(phase + x * 0.8) * 3.5 + 3.5;
      int sineY = round(sineValue);
      matrix.setPoint(x, sineY, true);  // Plot the sine wave

      // Cosine wave
      float cosineValue = cos(phase + x * 0.8) * 3.5 + 3.5;
      int cosineY = round(cosineValue);
      matrix.setPoint(x, cosineY, true);  // Plot the cosine wave
    }

    phase += 0.1;  // Increment phase for smooth motion
    if (phase > TWO_PI) phase -= TWO_PI;  // Reset phase after full cycle
  }
}

void graphPattern() {
  static int values[8] = {0};  // Array to store graph values for each column
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Shift values to the left (like a scrolling graph)
    for (int i = 1; i < 8; i++) {
      values[i - 1] = values[i];
    }

    // Generate a new random value for the last column
    values[7] = random(0, 8);  // Generate a random value between 0 and 7

    // Plot the graph values
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < values[x]; y++) {
        matrix.setPoint(x, 7 - y, true);  // Set the bars for the graph from bottom up
      }
    }
  }
}

void rippleWavePattern() {
  static float radius = 0.0;  // Radius of the ripple wave
  static unsigned long previousMillis = 0;
  const long interval = speed_val;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the current time
    matrix.clear();  // Clear the display

    // Draw the expanding ripple wave
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        float distance = sqrt(pow(x - 3.5, 2) + pow(y - 3.5, 2));  // Distance from center
        if (distance > radius - 0.5 && distance < radius + 0.5) {
          matrix.setPoint(x, y, true);  // Set points on the ripple wave
        }
      }
    }

    radius += 0.2;  // Increment radius for expansion
    if (radius > 6) radius = 0;  // Reset ripple once it reaches the edge
  }
}

//3d
void rotatingCube() {
    static int i = 0;
    static unsigned long previousMillis = 0;
    const long interval = speed_val;

    // Define a simple pattern for a rotating cube effect
    const bool cube[8][8] = {
        {0,0,0,0,0,0,0,0},
        {0,1,1,1,1,0,0,0},
        {0,1,0,0,1,0,0,0},
        {0,1,1,1,1,0,0,0},
        {0,0,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    };

    if (millis() - previousMillis >= interval) {
        previousMillis = millis();
        matrix.clear();  // Clear the display

        // Draw the cube
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (cube[row][col]) {
                    matrix.setPoint((col + i) % 8, (row + i) % 8, true);
                }
            }
        }

        // Update the position
        i++;
        if (i >= 8) i = 0;  // Loop back to the top
    }
}

void spiralEffect() {
    static int i = 0;
    static unsigned long previousMillis = 0;
    const long interval = speed_val;
    static int layer = 0;

    // Draw a spiral effect
    if (millis() - previousMillis >= interval) {
        previousMillis = millis();
        matrix.clear();  // Clear the display

        // Draw the spiral
        for (int i = 0; i < layer; i++) {
            matrix.setPoint(i, 0, true);                  // Left
            matrix.setPoint(0, i, true);                  // Top
            matrix.setPoint(i, layer - 1, true);          // Right
            matrix.setPoint(layer - 1, i, true);          // Bottom
        }

        layer++;
        if (layer >= 8) layer = 1;  // Reset layer after reaching max
    }
}

void tunnelEffect() {
    static unsigned long previousMillis = 0;
    const long interval = speed_val;
    static int i = 0;

    // Create a tunnel effect
    if (millis() - previousMillis >= interval) {
        previousMillis = millis();
        matrix.clear();  // Clear the display

        // Draw the tunnel
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if ((row + col) % 2 == 0) {
                    matrix.setPoint((col + i) % 8, row, true);
                }
            }
        }

        // Update position for the tunnel effect
        i++;
        if (i >= 8) i = 0;  // Reset position after reaching max
    }
}

void waveEffect3d() {
    static unsigned long previousMillis = 0;
    const long interval = speed_val;
    static int offset = 0;

    // Create a wave effect
    if (millis() - previousMillis >= interval) {
        previousMillis = millis();
        matrix.clear();  // Clear the display

        // Draw the wave
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if ((col + offset) % 4 == 0) {
                    matrix.setPoint(col, row, true);
                }
            }
        }

        // Update the offset for the wave effect
        offset++;
        if (offset >= 4) offset = 0;  // Reset offset after reaching max
    }
}


