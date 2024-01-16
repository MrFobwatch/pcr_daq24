#include <Arduino.h>

// Accelerometer declarations
const int xpin = A4;
const int ypin = A5;
const int zpin = A6;

// LED declarations
const bool LED = 7;

void setup() {
  // Initialize the serial communications:
  Serial.begin(9600);
}

void loop() {
  Serial.print(analogRead(xpin));

  Serial.print("\t");
  Serial.print(analogRead(ypin));

  Serial.print("\t");
  Serial.print(analogRead(zpin));

  Serial.print("\t");
  delay(100);
}

}