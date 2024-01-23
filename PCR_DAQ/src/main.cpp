#include <Arduino.h>
#include <VescUart.h>

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

// VescUart Class
VescUart UART;

void setup() {
	// Initialize the serial communications:
	Serial.begin(115200);

	// set the digital pin as output:
	pinMode(LED, OUTPUT);

	//Enable Serial on a port for UART
	Serial1.begin(115200);

	while (!Serial1) {;}

	// Set the port for the UART communication to the VESC
	UART.setSerialPort(&Serial1);
}

void loop() {
	// print the sensor values:
	Serial.print(">xaxis:");
	Serial.println(analogRead(xpin));
	// print a tab between values:
	//  Serial.print("\n");
	// print the sensor values:
	Serial.print(">yaxis:");
	Serial.println(analogRead(ypin));
	// print a tab between values:
	// Serial.print("\n");
	// print the sensor values:
	Serial.print(">zaxis:");
	Serial.println(analogRead(zpin));
	// print a tabe between values:
	// Serial.print("\n");

	weaponValueIn = analogRead(weaponCurrent);
	weaponValueOut = map(weaponValueIn, 0, 1023, 0, 255);

	if(weaponValueOut <= 512){
		ledState = HIGH;
	} else {
		ledState = LOW;
	}
	digitalWrite(LED, ledState);

	// Poll the VESC for data
	UART.getVescValues();

	// delay before next reading:
	delay(10);
}