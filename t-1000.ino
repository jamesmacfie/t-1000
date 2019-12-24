/*  The LCD circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * 
 * Wiring:
    // Pin 8 to IN1 on the ULN2003 driver
    // Pin 9 to IN2 on the ULN2003 driver
    // Pin 10 to IN3 on the ULN2003 driver
    // Pin 11 to IN4 on the ULN2003 driver
  */
    
    /* Example sketch to control a 28BYJ-48 stepper motor with ULN2003 driver board and Arduino UNO. More info: https://www.makerguides.com */
// Include stepper and LCD libraries
#include <Stepper.h>
#include <LiquidCrystal.h>

// Define number of steps per rotation for the stepper
const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

// Create our LCD
LiquidCrystal lcd(6, 7, 2, 3, 4, 5);

// State variables
bool isBrewing = false;
long millisecondsToBrew = 120000;
long brewStartTime;

void setup() {
  Serial.begin(9600);
  
  setupStepper();
  setupLCD();
}

void setupStepper() {
  // Set the speed to 5 rpm:
  myStepper.setSpeed(5);
}

void setupLCD() { 
  // Set the column/row counts of the LCD
  lcd.begin(16, 2);
}

void loop() {
  checkButtonPress();
  displayState();
  displayBrewTime();
  checkBrewFinish();
  delay(100); 
}

void checkButtonPress() {
  int x;
  x = analogRead(0);
  lcd.setCursor(0,1);
  
  if (x < 60) {
    // Right 
    addSecond();
  } else if (x > 100 && x < 150) {
    // Up
    moveStepperUp();
  } else if (x > 290 && x < 320){
    // Down
    moveStepperDown();
  } else if (x < 600){
    // Left
   removeSecond();
  } else if (x < 800) {
    // Select
    if (isBrewing) {
      stopBrew();
    } else {
      startBrew();  
    }
  }
}

void addSecond() {
  Serial.println(millisecondsToBrew);
  millisecondsToBrew = millisecondsToBrew + 10000;
  Serial.println(millisecondsToBrew);
}

void removeSecond() {
  long newMilliseconds = millisecondsToBrew - 10000;
  if (newMilliseconds > 0) {
    millisecondsToBrew = newMilliseconds;
  }
}

void moveStepperUp() {
  myStepper.step(stepsPerRevolution / 30);
} 

void moveStepperDown() {
  myStepper.step(stepsPerRevolution / 30 * -1);
}

void startBrew() {
  isBrewing = true;
  myStepper.step(stepsPerRevolution / 8 * -1);
  brewStartTime = millis();
}

void stopBrew() {
  isBrewing = false;
  myStepper.step(stepsPerRevolution / 8);
}

void displayState() {
  lcd.setCursor(0,0);
  if (isBrewing) {
    lcd.write("Brew in progress");
  } else {
    lcd.write("Not brewing yet ");
  }
}

void displayBrewTime() {
  char buffer[6];
  int minutes;
  int seconds;
  lcd.setCursor(0,1);
  if (isBrewing) {
    lcd.write("Remaining     ");
    lcd.setCursor(11,1);
    long timeRemaining = millisecondsToBrew - (millis() - brewStartTime);
    minutes = timeRemaining / 60000;
    seconds = (timeRemaining % 60000) / 1000;
  } else {
    lcd.write("Time     ");
    lcd.setCursor(11,1);
    minutes = millisecondsToBrew / 60000;
    seconds = (millisecondsToBrew % 60000) / 1000;
  }

  sprintf(buffer, "%02d:%02d", minutes, seconds);
  lcd.write(buffer);
}

void checkBrewFinish() {
   if (!isBrewing) {
    return;
   }

   long timeRemaining = millisecondsToBrew - (millis() - brewStartTime);
   Serial.println(timeRemaining);
   if (timeRemaining < 0) {
    lcd.setCursor(11,1);
    lcd.write("--:--");
    stopBrew();
   }
   
}
