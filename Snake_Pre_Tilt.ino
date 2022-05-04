#include <FastLED.h>

/* Game order
    1. Start Game Button:
    2a Generate kill border
    2b Generate Snake and Food randomly avoiding kill border
    2c Wait for movement command
    3a Move snake in direction
    3b Change snake direction
    3c set up board matrix to simplify this
    4a extend snake if food is eaten
      (check if snake eats food by checking if snake head is on same coordinate as food)
    4b kill snake if snake head touches border
    4c kill snake if snake head touches self
      (check for equal values in array)
    4d decrease movement delay if food is eaten
    4e generate new food if food is eaten
    5a end game if snake dies
    5b show your and hi score
    6. Restart to step 1

*/

//constants
#define button 2
#define matrixPin 3
#define tiltUp 4
#define tiltDown 5
#define tiltLeft 6
#define tiltRight 7
#define NUM_LEDS 256

//Declare variables
CRGB leds[NUM_LEDS];
int snakeLength;
int snakeArray[30];
int snakeHead[] = {10, 5};
int food;
short moveDirection = 1;
float moveDelay;
float pastTime = 0;
bool gameOn = false;
bool gameOver = false;
//layout of 16x16 Sainsmart matrix
int matrixNums [16][16] = {{0, 31, 32, 63, 64, 95, 96, 127, 128, 159, 160, 191, 192, 223, 224, 255},
  {1, 30, 33, 62, 65, 94, 97, 126, 129, 158, 161, 190, 193, 222, 225, 254},
  {2, 29, 34, 61, 66, 93, 98, 125, 130, 157, 162, 189, 194, 221, 226, 253},
  {3, 28, 35, 60, 67, 92, 99, 124, 131, 156, 163, 188, 195, 220, 227, 252},
  {4, 27, 36, 59, 68, 91, 100, 123, 132, 155, 164, 187, 196, 219, 228, 251},
  {5, 26, 37, 58, 69, 90, 101, 122, 133, 154, 165, 186, 197, 218, 229, 250},
  {6, 25, 38, 57, 70, 89, 102, 121, 134, 153, 166, 185, 198, 217, 230, 249},
  {7, 24, 39, 56, 71, 88, 103, 120, 135, 152, 167, 184, 199, 216, 231, 248},
  {8, 23, 40, 55, 72, 87, 104, 119, 136, 151, 168, 183, 200, 215, 232, 247},
  {9, 22, 41, 54, 73, 86, 105, 118, 137, 150, 169, 182, 201, 214, 233, 246},
  {10, 21, 42, 53, 74, 85, 106, 117, 138, 149, 170, 181, 202, 213, 234, 245},
  {11, 20, 43, 52, 75, 84, 107, 116, 139, 148, 171, 180, 203, 212, 235, 244},
  {12, 19, 44, 51, 76, 83, 108, 115, 140, 147, 172, 179, 204, 211, 236, 243},
  {13, 18, 45, 50, 77, 82, 109, 114, 141, 146, 173, 178, 205, 210, 237, 242},
  {14, 17, 46, 49, 78, 81, 110, 113, 142, 145, 174, 177, 206, 209, 238, 241},
  {15, 16, 47, 48, 79, 80, 111, 112, 143, 144, 175, 176, 207, 208, 239, 240}
};


void setup()
{
  //Declare LED setup
  FastLED.addLeds<WS2812, matrixPin, RGB>(leds, NUM_LEDS);
  //tilt sensor and button pins
  pinMode(tiltUp, INPUT);
  pinMode(tiltDown, INPUT);
  pinMode(tiltLeft, INPUT);
  pinMode(tiltDown, INPUT);
  pinMode(button, INPUT);
}
void loop()
{

  if (gameOn == false) {
    //set up board and variables
    //Place snake head and food on board
    gameSetup();

    //  //dont start game until button is pressed
    //  while(1) {
    //    if(digitalRead(button) == HIGH) {
    //      break;
  }
  //  }
  //}
  delay(2000);
  for (int i = 0; i < 10; i++) {
    for (int j = snakeLength; j > 0; j--) {
      snakeArray[j] = snakeArray[j - 1];
    }
    if (i % 2 == 0) {
      snakeHead[0] = snakeHead[0] - 1;
      snakeArray[0] = matrixNums[snakeHead[0]][snakeHead[1]];
    }
    else {
      snakeHead[1] = snakeHead[1] - 1;
      snakeArray[0] = matrixNums[snakeHead[0]][snakeHead[1]];
    }
    leds[snakeArray[0]] = CHSV(20, 200, 70);
    leds[snakeArray[snakeLength]] = CHSV(0, 0, 0);
    FastLED.show();
    checks();
    if (gameOver == true) {
      break;
    }
  }
  gameEnd();

}

void gameSetup() {
  LEDAllOff();
  gameOver = false;
  gameOn = true;
  //set up kill border
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      if (i == 0 || i == 15 || j == 0 || j == 15) {
        int l = matrixNums[i][j];
        leds[l] = CHSV(99, 220, 70);
      }
    }
  }
  moveDelay = 800000;
  snakeLength = 5;
  //Spawn snake head at pin 85
  leds[85] = CHSV(20, 200, 70);
  snakeArray[0] = 85;
  generateFood();
  FastLED.show();
}

void generateFood() {
  //randomly generate food until empty space found
  do {
    randomSeed(analogRead(0));
    food = random(255);
  } while (leds[food] != CHSV(0, 0, 0));
  leds[food] = CHSV(160, 250, 70);
}

void LEDAllOff() {
  //Sets all LEDs off
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, 0, 0);
  }
  FastLED.show();
}

void checks() {
  if (snakeArray[0] == food) {
    snakeLength++;
    moveDelay = moveDelay * 0.9;
    generateFood();
  }
  if (snakeHead[0] == 0 || snakeHead[0] == 15 || snakeHead[1] == 0 || snakeHead[1] == 15) {
    gameOver == true;
    gameOn == false;
  }
  for (int i = 1; i <= snakeLength; i++) {
    if (snakeArray[0] == snakeArray[i]) {
      gameOver == true;
      gameOn == false;
      break;
    }
  }
}

void gameEnd() {
  for (int x = 0; x < 7; x++) {
    if (x % 2 == 0) {
      for (int i = 0; i < snakeLength; i++) {
        leds[snakeArray[i]] = CHSV(0, 0, 0);
      }
      FastLED.show();
    }
    else {
      for (int i = 0; i < snakeLength; i++) {
        leds[snakeArray[i]] = CHSV(20, 200, 70);
      }
      FastLED.show();
    }
  }
}
