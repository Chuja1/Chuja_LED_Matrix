#include <Keyboard.h>
#include <KeyboardLayout.h>
#include <Mouse.h>
const int myScreenWidth   = 1920;       // screen width
const int myScreenHeight  =  1080;       // screen height
volatile unsigned long tickCount;
#define TicksPerRotaion 22
#define rpmThresh 2
unsigned long currentTime;
unsigned long prevTime;
int tmpTickCount;
int rotationChange;
int cScheme = 1;


void setup() {
  // make pin 2 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  attachInterrupt(digitalPinToInterrupt(0), rotationCount , FALLING);
  tickCount = 0;
  currentTime = 0;
  prevTime = 0;
  tmpTickCount = 0;


  pinMode(2, INPUT);//2-6 control buttons
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);//control scheme button
  // initialize control over the keyboard:
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  Serial.println(rotationChange);
  unsigned long currentTime = micros() - prevTime;
  if (digitalRead(7) == HIGH) {
    delay(100);
    if (digitalRead(7) == HIGH) {
      changeScheme();
    }
  }

  if (currentTime > 100000) {
    prevTime = micros();
    rotationChange = tickCount - tmpTickCount;
    tmpTickCount = tickCount;
    
  }
  if (rotationChange > rpmThresh) {
    switch (cScheme) {
      //first case is subway surfer controls
      case 1:
        if (digitalRead(2) == HIGH) {
          Mouse.press();
          for (int i = 0; i < 10; i++) {
            Mouse.move(-20, 0); //left
            delay(15);
          }
          Mouse.release();
          delay(200);
          moveMouse2screenCenter();
        }
        if (digitalRead(3) == HIGH) {
          Mouse.press();
          for (int i = 0; i < 10; i++) {
            Mouse.move(0, 20); //down
            delay(15);
          }
          Mouse.release();
          delay(200);
          moveMouse2screenCenter();
        }
        if (digitalRead(4) == HIGH) {
          Mouse.press();
          for (int i = 0; i < 10; i++) {
            Mouse.move(0, -20); //up
            delay(15);
          }
          Mouse.release();
          delay(200);
          moveMouse2screenCenter();
        }
        if (digitalRead(5) == HIGH) {
          Mouse.press();
          for (int i = 0; i < 10; i++) {
            Mouse.move(20, 0); //right
            delay(15);
          }
          Mouse.release();
          delay(200);
          moveMouse2screenCenter();
        }
        break;
      //second case is wasd space control
      case 2:
        if (digitalRead(2) == HIGH) {
          Keyboard.write('a');
          delayMicroseconds(200);
        }
        else if (digitalRead(3) == HIGH) {
          Keyboard.write('s');
          delayMicroseconds(200);
        }
        else if (digitalRead(4) == HIGH) {
          Keyboard.write('w');
          delayMicroseconds(200);
        }
        else if (digitalRead(5) == HIGH) {
          Keyboard.write('d');
          delayMicroseconds(200);
        }
        if (digitalRead(6) == HIGH) {
          Keyboard.write(' ');
          delayMicroseconds(200);
        }
        break;
        //general mouse and left click control
      case 3: 
        if (digitalRead(2) == HIGH) {
          Mouse.move(-20, 0); //move mouse left
          delay(25);
        }
        else if (digitalRead(3) == HIGH) {
          Mouse.move(0, 20); //move mouse down
          delay(25);
        }
        else if (digitalRead(4) == HIGH) {
          Mouse.move(0, -20); //move mouse up
          delay(25);
        }
        else if (digitalRead(5) == HIGH) {
          Mouse.move(20, 0); //move mouse right
          delay(25);
        }
        else if (digitalRead(6) == HIGH) {
          Mouse.click();
          delay(200);
        }
        break;
    }
  }
}

void moveMouse2screenCenter() {
  int moveXtimes = (myScreenWidth / 127 / 2);
  int moveYtimes = (myScreenHeight / 127 / 2);
  signed char lastMoveX = (myScreenWidth / 2) - (moveXtimes * 127);
  signed char lastMoveY = (myScreenHeight / 2) - (moveYtimes * 127);
  moveMouse2UpperLeftCorner();
  for (int i = 0; i < moveXtimes; i++) {
    Mouse.move(127, 0, 0);
  }
  Mouse.move(lastMoveX, 0, 0);
  for (int i = 0; i < moveYtimes; i++) {
    Mouse.move(0, 127, 0);
  }
  Mouse.move(0, lastMoveY, 0);
}

void moveMouse2UpperLeftCorner() {
  int moveXtimes = (myScreenWidth / 127) + 1;
  int moveYtimes = (myScreenHeight / 127) + 1;
  for (int i = 0; i < moveXtimes; i++) {
    Mouse.move(-127, 0, 0);
  }
  for (int i = 0; i < moveYtimes; i++) {
    Mouse.move(0, -127, 0);
  }
}

void rotationCount()
{
  tickCount++;
}

void changeScheme() {
  if (cScheme == 1) {
    cScheme = 2;
  }
  else if (cScheme == 2) {
    cScheme = 3;
  }
  else if (cScheme == 3) {
    cScheme = 1;
  }
}
