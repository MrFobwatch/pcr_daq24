#include <Arduino.h>

// Accelerometer declarations
const int xpin = A4;
const int ypin = A5;
const int zpin = A6;

// LED declarations
const int LED = 7;
int ledState = LOW;

// Weapon Speed based off current
const int weaponCurrent = A0;
int weaponValueIn = 0;
int weaponValueOut = 0;

void setup() {
  // Initialize the serial communications:
  Serial.begin(9600);

  // set the digital pin as output:
  pinMode(LED, OUTPUT);
}

void loop() {
  // print the sensor values:
  Serial.print(analogRead(xpin));
  // print a tab between values:
  Serial.print("\t");
  // print the sensor values:
  Serial.print(analogRead(ypin));
  // print a tab between values:
  Serial.print("\t");
  // print the sensor values:
  Serial.print(analogRead(zpin));
  // print a tabe between values:
  Serial.print("\t");

  weaponValueIn = analogRead(weaponCurrent);
  weaponValueOut = map(weaponValueIn, 0, 1023, 0, 255);

  if(weaponValueOut <= 512){
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(LED, ledState);

  // delay before next reading:
  delay(10);
}
