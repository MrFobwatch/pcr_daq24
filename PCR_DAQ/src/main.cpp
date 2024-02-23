#include <Arduino.h>
#include <VescUart.h>

// Accelerometer declarations
const int xpin = A4;
const int ypin = A5;
const int zpin = A6;

// LED declarations
const int LED = 7;
int ledState = LOW;

// Weapon Variables
// const int weaponCurrent = A0;
int weaponValueIn = 0;
int weaponValueOut = 0;

long weaponRPM = 0;
float weaponCurrent = 0;
float weaponInputCurrent = 0;

// Drivetrain Variables

long leftDriveRPM;
float leftDriveCurrent, leftDriveInputCurrent;

long rightDriveRPM;
float rightDriveCurrent, rightDriveInputCurrent;

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
	// Serial.print(">xaxis:");
	// Serial.println(analogRead(xpin));
	// print a tab between values:
	//  Serial.print("\n");
	// print the sensor values:
	// Serial.print(">yaxis:");
	// Serial.println(analogRead(ypin));
	// print a tab between values:
	// Serial.print("\n");
	// print the sensor values:
	// Serial.print(">zaxis:");
	// Serial.println(analogRead(zpin));
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

	// Poll the directly attached VESC for data
	UART.getVescValues();

	weaponRPM = UART.data.tachometer;
	weaponCurrent = UART.data.avgMotorCurrent;
	weaponInputCurrent = UART.data.avgInputCurrent;

	// Poll the drive VESC for data (CAN ID of Drive VESC)
	UART.getVescValues(2); 
	leftDriveRPM = UART.data.tachometer;
	leftDriveCurrent = UART.data.avgMotorCurrent;
	leftDriveInputCurrent = UART.data.avgInputCurrent;

	// Poll the other drive VESC
	UART.getVescValues(3); 
	rightDriveRPM = UART.data.tachometer;
	rightDriveCurrent = UART.data.avgMotorCurrent;
	rightDriveInputCurrent = UART.data.avgInputCurrent;

	//Test UART connection

	bool vescdata = UART.getVescValues();
	Serial.print(">Bool:");
	Serial.println(vescdata);

	Serial.print(">VESCTemp:");
	Serial.println(UART.data.tempMosfet);

	Serial.print(">VESCinpVolt:");
	Serial.println(UART.data.inpVoltage);

	// delay before next reading:
	delay(10);
}