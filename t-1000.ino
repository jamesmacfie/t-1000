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
  lcd.setCursor(0,0);
  lcd.print("Electropeak.com");
  lcd.setCursor(0,1);
  lcd.print("Press Key:");
}

void loop() {
  checkButtonPress();
  // Step one revolution in one direction:
//  Serial.println("clockwise");
//  myStepper.step(stepsPerRevolution);
//  delay(500);
//  
//  // Step one revolution in the other direction:
//  Serial.println("counterclockwise");
//  myStepper.step(-stepsPerRevolution);
//  delay(500);
}

void checkButtonPress() {
  int x;
  x = analogRead(0);
  lcd.setCursor(0,1);
  if (x < 60) {
    // Right 
    lcd.print ("Right ");
  } else if (x < 200) {
    // Up
    moveStepperUp();
  } else if (x < 400){
    // Down
    moveStepperDown();
  } else if (x < 600){
    // Left
   lcd.print ("Left ");
  } else if (x < 800) {
    // Select
    if (isBrewing) {
      stopBrew();
    } else {
      startBrew();  
    }
  }
}

void moveStepperUp() {
  Serial.println("Moving stepper up");
  myStepper.step(stepsPerRevolution / 30);
} 

void moveStepperDown() {
  Serial.println("Moving stepper down");
  myStepper.step(stepsPerRevolution / 30 * -1);
}

void startBrew() {
  Serial.println("Starting brew");
  isBrewing = true;
  myStepper.step(stepsPerRevolution / 4 * -1);
}

void stopBrew() {
  Serial.println("Stopping brew");
  isBrewing = false;
  myStepper.step(stepsPerRevolution / 4);
}
