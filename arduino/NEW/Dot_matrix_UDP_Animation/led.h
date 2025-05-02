#ifndef LED_H
#define LED_H
#include <MD_MAX72xx.h>

// Declare matrix as extern to use it across multiple files
extern MD_MAX72XX matrix;
extern int speed_val;

// Declare the function prototype
void setupLED();
void setBrightness(int);
int format_string(String);
int format_string_anim(String req);
void bouncingBall();
void blinkCorners();
void diagonalLine();
void movingLine();
void zigzagPattern();
void flashingSquare();
void spiralPattern();
void rainEffect();
void wavePattern();
void rotatingSquare();
void fireEffect();
void starfield();
void plasmaEffect();
void fireworksEffect();
void sineWavePattern();
void cosineWavePattern();
void dualWavePattern();
void graphPattern();
void rippleWavePattern();
void rotatingCube();
void spiralEffect();
void tunnelEffect();
void waveEffect3d();

void setSpeed(int);

#endif