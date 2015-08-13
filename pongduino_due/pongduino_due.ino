//#include <Pongduino.h>

#include "HT1632_DUE.h"
#include "Wire.h"
#include "ArduinoNunchuk.h"
#include "ArduinoNunchuk2.h"

// #include <string.h>
// #include <stdio.h>

#define DATA 4
#define WR   5
#define CS   6

// for width 24, height 16 panel
HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);
ArduinoNunchuk leftChuk;
ArduinoNunchuk2 rightChuk;


const int padLength = 5;
const float startingBottom = 9; //starting Y position for paddle bottoms
const int leftPadX = 0;
const int rightPadX = 23;
const float minSpeed = 100; // divided by 1000
const float maxSpeed = 1000; // divided by 1000

unsigned long startTime = millis();

float rightPadSpeed = 0.5;
float leftPadSpeed = 0.5;

float leftPadBottom = startingBottom;
float rightPadBottom = startingBottom;

float ballX = 0;
float ballY = 0;
float ballXspeed = 0;
float ballYspeed = 0;

int leftPlayerScore = 0;
int rightPlayerScore = 0;


void setup() {
  Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  matrix.begin(HT1632_COMMON_16NMOS);
  randomSeed(analogRead(0));
  titleScreen();
  initPad(true); // init left paddle
  initPad(false); // init right paddle
  serveBall((int)random(2));
  initNunchuks();
  matrix.writeScreen();
  delay(500);
}


void loop() {
  // put your main code here, to run repeatedly:
  /*
  if ((millis()-startTime) > 2000) {
    serveBall((int)random(2));
    startTime = millis();
  }
  */
  updateNunchuks();
  movePadsWithNunchuk();
  moveBall();
  initPad(true);
  initPad(false);
  
/*
  if ((int)random(2)) {
    movePadDown((int)random(2));
  }
  if ((int)random(2)) {
    movePadUp((int)random(2));
  }
*/
  matrix.writeScreen();
  delay(20);
}

void titleScreen() {
  matrix.setTextSize(1);
  matrix.setTextColor(1);
  matrix.setCursor(0, 0);   // start at top left, with one pixel of spacing
  matrix.print("PLAY");
  for (int x = 0; x < 3; x++) {
    matrix.setTextColor(0);
    matrix.setCursor(0, 8);   // next line, 8 pixels down
    matrix.print("PONG");
    matrix.writeScreen();
    delay(500);
    matrix.setTextColor(1);
    matrix.setCursor(0, 8);   // next line, 8 pixels down
    matrix.print("PONG");
    matrix.writeScreen();
    delay(500);
  }
  delay(500);
  matrix.clearScreen();
  delay(500);
}

void displayWin(bool left) {
  matrix.clearScreen();
  matrix.setTextSize(1);
  matrix.setTextColor(1);
  matrix.setCursor(0, 0);   // start at top left, with one pixel of spacing
  if (left == 1) {
    matrix.print("PL 1");
  }
  else {
    matrix.print("PL 2");
  }
  for (int x = 0; x < 3; x++) {
    matrix.setTextColor(0);
    matrix.setCursor(0, 8);   // next line, 8 pixels down
    matrix.print("WINS");
    matrix.writeScreen();
    delay(500);
    matrix.setTextColor(1);
    matrix.setCursor(0, 8);   // next line, 8 pixels down
    matrix.print("WINS");
    matrix.writeScreen();
    delay(500);
  }
  delay(500);
  matrix.clearScreen();
  delay(500);
  titleScreen();
  initPad(true);
  initPad(false);
}

void initPad(bool leftPad) {
  int padBottom = leftPadBottom;
  int padX = leftPadX;
  if (leftPad == false) {
    padBottom = rightPadBottom;
    padX = rightPadX;
  }
//  if (padBottom != startingBottom) { //clear old pixels and reset
    for (int i = padBottom; i > padBottom - padLength; i--) {
      matrix.clrPixel(padX, i);
//    }
//    if (leftPad == false) rightPadBottom = startingBottom;
//    if (leftPad == true) leftPadBottom = startingBottom;
  }
  for (int i = padBottom; i > padBottom - padLength; i--) {
    matrix.setPixel(padX, i);
  }
}

void movePadUp(bool leftPad) {
  float padBottom = leftPadBottom;
  int padX = leftPadX;
  float padSpeed = leftPadSpeed;
  if (leftPad == false) {
    padBottom = rightPadBottom;
    padX = rightPadX;
    padSpeed = rightPadSpeed;
  }
  float newPadBottom = padBottom - padSpeed;
  //Serial.println(newPadBottom);
  if (newPadBottom >= padLength - padSpeed) { // paddle won't go off edge, ok to move it
    //Serial.print("not going off top") ;
    for (int i = 0; i < 16; i++) {
      matrix.clrPixel(padX, i);
    }
    /*
    for (int i = round(padBottom); i > (round(padBottom) - padLength); i--) {
      matrix.clrPixel(padX, i);
    }
    */
    padBottom = newPadBottom;
    for (int i = padBottom; i > padBottom - padLength; i--) {
      matrix.setPixel(padX, i);
    }
    if (leftPad == false) {
      rightPadBottom = padBottom;
    }
    if (leftPad == true) {
      leftPadBottom = padBottom;
    }
  }
}

void movePadDown(bool leftPad) {
  float padBottom = leftPadBottom;
  int padX = leftPadX;
  float padSpeed = leftPadSpeed;
  if (leftPad == false) {
    padBottom = rightPadBottom;
    padX = rightPadX;
    padSpeed = rightPadSpeed;
  }
  float newPadBottom = padBottom + padSpeed;
  //Serial.println(newPadBottom);
  if (round(newPadBottom) > 15) {
    newPadBottom = 15;
  }
  if (round(newPadBottom) < 16) { // paddle won't go off edge, ok to move it
    for (int i = 0; i < 16; i++) {
      matrix.clrPixel(padX, i);
    }
    //Serial.println("not going off bottom") ;
    /*
    for (int i = padBottom; i > padBottom - padLength; i--) {
      matrix.clrPixel(padX, i);
    }
    */
    padBottom = newPadBottom;
    for (int i = padBottom; i > padBottom - padLength; i--) {
      matrix.setPixel(padX, i);
    }
    if (leftPad == false) {
      rightPadBottom = padBottom;
    }
    if (leftPad == true) {
      leftPadBottom = padBottom;
    }
  }
}

void serveBall(bool left) {
  //matrix.clrPixel(round(ballX), round(ballY));
  //matrix.writeScreen();
  if (rightPlayerScore == 7) {
    displayWin(false);
    rightPlayerScore = 0;
    leftPlayerScore = 0;
  }
  else if (leftPlayerScore == 7) {
    displayWin(true);
    rightPlayerScore = 0;
    leftPlayerScore = 0;    
  }
  if (left == true) {
    ballX = 3;
    ballY = leftPadBottom - padLength / 2;
    ballXspeed = (float)random(minSpeed, (maxSpeed / 2) + 1) / 1000;
  }
  if (left == false) {
    ballX = 20;
    ballY = rightPadBottom - padLength / 2;
    ballXspeed = (float)random(-1 * (maxSpeed / 2), (-1 * minSpeed) + 1) / 1000;
    //Serial.print("rightballXspeed: ");
    //Serial.println(ballXspeed);
  }
  long randomNumber = 0;
  while (abs(randomNumber) < minSpeed) {
    randomNumber = random(-1 * (maxSpeed / 2), (maxSpeed / 2) + 1);
    ballYspeed = (float)(randomNumber) / 1000;
  }
  matrix.setPixel(round(ballX), round(ballY));
}

void moveBall() {
  float newBallX = ballX + ballXspeed;
  float newBallY = ballY + ballYspeed;
  if ((round(newBallX) != round(ballX) || round(newBallY) != round(ballY))) {
    // left paddle bounce
    if ((round(newBallX) == leftPadX) && (round(newBallY) <= round(leftPadBottom)) && (round(newBallY) >= round(leftPadBottom - padLength))) {
      ballXspeed = -1.1 * ballXspeed;
      ballYspeed = 1.1 * ballYspeed;
      newBallX = ballX + ballXspeed;
    }
    // right paddle bounce
    if ((round(newBallX) == rightPadX) && (round(newBallY) <= round(rightPadBottom)) && (round(newBallY) >= round(rightPadBottom - padLength))) {
      ballXspeed = -1.1 * ballXspeed;
      ballYspeed = 1.1 * ballYspeed;
      newBallX = ballX + ballXspeed;
    }
    // bottom or top bounce
    if (round(newBallY) < 0 || round(newBallY) > 15)  {
      ballYspeed = -1 * ballYspeed;
      newBallY = ballY + ballYspeed;
    }

    matrix.clrPixel(round(ballX), round(ballY));
  }
  
  if (round(newBallX) >= 0 && round(newBallX) < 24) {
    matrix.setPixel(round(newBallX), round(newBallY));

    ballX = newBallX;
    ballY = newBallY;
  }
  // left wall score for rightPlayer
  else if (round(newBallX) < 0) {
    initPad(true);
    drawScores(250);
    rightPlayerScore++;
    drawScores(500);
    serveBall(true);
  }

  // right wall score for leftPlayer
  else if (round(newBallX) > 23) {
    initPad(false);
    drawScores(250);
    leftPlayerScore++;
    drawScores(500);
    serveBall(false);
  }

}

void drawScores(unsigned long delayMS) {
  matrix.setTextSize(1);
  matrix.setTextColor(1);
  matrix.setCursor(4, 4);   
  matrix.print(leftPlayerScore);
  matrix.setCursor(15, 4);
  matrix.print(rightPlayerScore);
  matrix.writeScreen();
  delay(delayMS);
  matrix.setTextColor(0);
  matrix.setCursor(4, 4); 
  matrix.print(leftPlayerScore);
  matrix.setCursor(15, 4);
  matrix.print(rightPlayerScore);
  matrix.writeScreen();
   
}

void initNunchuks() {
  leftChuk.init();
  rightChuk.init();
}

void   updateNunchuks() {
  leftChuk.update();
  rightChuk.update();
  Serial.println(leftChuk.accelY);  
//  Serial.println(chuk2.accelY);  
  // ~535 is zero
}

float convertAccelToSpeed(int accel) {
  int zeroedAccel = accel - 535; // 535 is the value at level
  //zeroedAccel = abs(zeroedAccel);
  if(zeroedAccel <=50 && zeroedAccel >=-50) {
    return 0.0;
  }
  if(zeroedAccel > 50 && zeroedAccel <= 100) {
    return 0.50;
  }
  if (zeroedAccel < -50 && zeroedAccel >= -100) {
    return -0.50;
  }
  if(zeroedAccel > 100 && zeroedAccel <= 150) {
    return 0.75;
  }  
  if (zeroedAccel < -100 && zeroedAccel >= -150) {
    return -0.75;
  }
  if(zeroedAccel > 150 && zeroedAccel <= 200) {
    return 1.25;
  }   
  if(zeroedAccel < -150 && zeroedAccel >= -200) {
    return -1.25;
  } 
  if(zeroedAccel > 200) {
    return 2.50;
  }
  return -2.50;
}

void movePadsWithNunchuk() {
    rightPadSpeed = convertAccelToSpeed(rightChuk.accelY);
    if(rightPadSpeed>0) {
      movePadUp(0);
    }
    if(rightPadSpeed<0) {
      rightPadSpeed = abs(rightPadSpeed);
      movePadDown(0);
    } 
    leftPadSpeed = convertAccelToSpeed(leftChuk.accelY);
    Serial.println(leftPadSpeed);
    if(leftPadSpeed>0) {
      movePadUp(1);
    }
    if(leftPadSpeed<0) {
      leftPadSpeed = abs(leftPadSpeed);
      movePadDown(1);
    }  
}

