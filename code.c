#include <Wire.h>  // Include Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // Include LCD library

// Initialize IR sensor pins
const int irSensorEnterPin = 2;  // IR sensor for entering
const int irSensorExitPin = 3;   // IR sensor for exiting

// Initialize LED pins
const int greenLedPin = 4;  // Green LED for entering
const int redLedPin = 5;    // Red LED for exiting

// Initialize 7-segment display pins
const int segmentPins[7] = {6, 7, 8, 9, 10, 11, 12}; // a, b, c, d, e, f, g
const int digitPins[2] = {A0, A1}; // Common cathode/anode pins for digits

// Initialize LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address, columns, rows

// Global variables
int occupancyCount = 0;

void setup() {
  // Initialize IR sensor pins
  pinMode(irSensorEnterPin, INPUT);
  pinMode(irSensorExitPin, INPUT);

  // Initialize LED pins
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  // Initialize 7-segment display pins
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  for (int i = 0; i < 2; i++) {
    pinMode(digitPins[i], OUTPUT);
  }

  // Initialize LCD display
  lcd.init();
  lcd.backlight();

  // Initial display update
  updateDisplay();
}

void loop() {
  // Check IR sensor inputs
  if (digitalRead(irSensorEnterPin) == HIGH) {
    incrementCount();
    updateDisplay();
    delay(500);  // Debounce delay
  }
  
  if (digitalRead(irSensorExitPin) == HIGH) {
    decrementCount();
    updateDisplay();
    delay(500);  // Debounce delay
  }
}

void incrementCount() {
  digitalWrite(greenLedPin, HIGH); // Turn on the green LED
  occupancyCount++;
  delay(200);  // LED on time
  digitalWrite(greenLedPin, LOW);  // Turn off the green LED
}

void decrementCount() {
  digitalWrite(redLedPin, HIGH); // Turn on the red LED
  if (occupancyCount > 0) {
    occupancyCount--;
  }
  delay(200);  // LED on time
  digitalWrite(redLedPin, LOW);  // Turn off the red LED
}

void updateDisplay() {
  // Display occupancy count on 7-segment display
  for (int digit = 0; digit < 2; digit++) {
    digitalWrite(digitPins[digit], HIGH);  // Turn on the selected digit
    for (int segment = 0; segment < 7; segment++) {
      digitalWrite(segmentPins[segment], segmentPatterns[occupancyCount % 10][segment]);
    }
    delay(1);  // Display persistence
    digitalWrite(digitPins[digit], LOW);  // Turn off the selected digit
    occupancyCount /= 10; // Move to the next digit
  }

  // Display occupancy count on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Occupancy: ");
  lcd.print(occupancyCount);
}
