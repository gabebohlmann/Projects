#include <Stepper.h>
#include <LiquidCrystal.h>
#include <math.h>

// motor pins
const int motorIn1 = 10;
const int motorIn2 = 11;
const int motorIn3 = 12;
const int motorIn4 = 13;

// button input pins
const int startButtonPin = 18;
const int throttleButtonPin = 19;
const int pauseButtonPin = 20;
const int abortButtonPin = 21;

// LED output pins
const int ledRedPin = 2;
const int ledGreenPin = 3;
const int ledBluePin = 4;

//LCD output pins
const int rs = 32;
const int en = 30;
const int d4 = 28;
const int d5 = 26;
const int d6 = 24;
const int d7 = 22;

// motor parameters
const int stepsPerRevolution = 200;
const int startRPM = 5;
const int throttleUpRPM = 30;
const int maxSteps = 535;
int currentStep = 0;
int maxThrottleSteps = maxSteps;

//button states
int startState = LOW;
int throttleState = LOW;
int pauseState = LOW;
int abortState = LOW;


Stepper motor = Stepper(stepsPerRevolution, motorIn1, motorIn2, motorIn3, motorIn4);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(startButtonPin), startInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(throttleButtonPin), throttleInterrupt, RISING);
  lcd.begin(16, 2);
  lcd.print("Time: 999s");
  //lcd.setCursor(0,1);
  //lcd.print("Throttle: 9000%");
  ledAlert("white", 25);
}

void loop() {
  if(startState == HIGH) {
    ledAlert("green", 10);
    startState == LOW;
    start();
    delay(30000);
  }  
  //delay(30000);
}

void start() {
  for(int i = 0; i < maxSteps; i++) { 
    if (throttleState == HIGH) { 
      throttleUp();
      break;
    } else {
      motor.step(-1); 
      currentStep++;
      if ((currentStep / 9) % 9 == 0) {
        lcd.setCursor(11, 1);
        lcd.print(getThrottlePercentage());
      } else if ((currentStep / 6) % 6 == 0) {
        lcd.setCursor(6, 1);
        lcd.print(getThrottlePercentage());
      } else if ((currentStep / 3) % 3 == 0) {
        lcd.setCursor(0, 1);
        lcd.print(getThrottlePercentage());
      }
      delay(50);
    }
  }
  
}

void throttleUp() {
  throttleState = LOW;
  int firstFastStep = currentStep;
  ledAlert("blue", 10);
  for(int i = currentStep; i < maxThrottleSteps; i++) {//firstFastStep
//    if(abortState == HIGH) {
//      abort();
//    } else {
      motor.step(-1);
      currentStep++;
      delay(10);
    //}
  }
}

void abort() {
  for(int i = currentStep; i > 0; i--) {
    motor.step(1);
    currentStep--;
    delay(5);
  }
}

void startInterrupt() {
  startState = HIGH;
}

void throttleInterrupt() {
  throttleState = HIGH;
}

void pauseInterrupt() {
  pauseState = HIGH;
}

void abortInterrupt() {
  abortState = HIGH;
}

void ledAlert(String color, int br) {
  if(color.equals("white")) {
    blinkLED(br, br, br, 75);
    lightLED(br, br,br);
  } else if(color.equals("red")) {
    blinkLED(br, 0, 0, 75);
    lightLED(br, 0, 0);
  } else if(color.equals("green")) {
    blinkLED(0, br, 0, 75);
    lightLED(0, br, 0);
  } else if(color.equals("blue")) {
    blinkLED(0, 0, br, 75);
    lightLED(0, 0, br);
  } 
}

void lightLED(int red, int green, int blue) {
  analogWrite(ledRedPin, red);
  analogWrite(ledGreenPin, green);
  analogWrite(ledBluePin, blue);
}

void blinkLED(int red, int green, int blue, int delayTime) {
  clearLED();
  lightLED(red, green, blue);
  delay(delayTime);
  clearLED();
  delay(delayTime);
  lightLED(red, green, blue);
  delay(delayTime);
  clearLED();
  delay(delayTime);
  lightLED(red, green, blue);
  delay(delayTime);
  clearLED();
  delay(delayTime);
}

void clearLED() {
  lightLED(0, 0, 0);
}

float getThrottlePercentage() {
  float percentage = (float(currentStep)/float(maxThrottleSteps))*100;
  return percentage;
}
