#include <Arduino.h>
#include <SD.h>
#include <VescUart.h>
#include <FastLED.h>

// Accelerometer declarations
// #define xpin = A4;
// #define ypin = A5;
// #define zpin = A6;

// LED declarations
const int LED_pin = 7;
#define DATA_PIN 3
#define NUM_LEDS 10
CRGB leds[NUM_LEDS];


int ledState = LOW;

// Motor/ESC Declarations
const int poles = 16;

// Weapon Variables
// const int weaponCurrent = A0;
int weaponValueIn = 0;
int weaponValueOut = 0;

long weaponElecRPM = 0;
float weaponCurrent = 0;
float weaponInputCurrent = 0;
int weaponCalcRPM;

// Drivetrain Variables

long leftDriveRPM;
float leftDriveCurrent, leftDriveInputCurrent;

long rightDriveRPM;
float rightDriveCurrent, rightDriveInputCurrent;

// VescUart Class
VescUart UART;

const int chipSelect = BUILTIN_SDCARD;

void setup() {
	// Initialize the serial communications:
	Serial.begin(115200);

	// set the digital pin as output:
	pinMode(LED_pin, OUTPUT);

	//Enable the led
	FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

	//Enable Serial on a port for UART
	Serial1.begin(115200);

	// Set the port for the UART communication to the VESC
	UART.setSerialPort(&Serial1);
	UART.setDebugPort(&Serial);
}

void loop() {

	// open the file named datalog.txt on the sd card
	File dataFile = SD.open("datalog.txt", FILE_WRITE);

	// Poll the directly attached VESC for data
	UART.getVescValues();

	weaponElecRPM = UART.data.rpm;
	weaponCalcRPM = (UART.data.rpm) / (poles / 2);
	Serial.print(">VESCcalcRPM:");
	Serial.println(weaponCalcRPM);
	weaponCurrent = UART.data.avgMotorCurrent;
	weaponInputCurrent = UART.data.avgInputCurrent;

	// Poll the drive VESC for data (CAN ID of Drive VESC)
	UART.getVescValues(2); 
	leftDriveRPM = UART.data.rpm;
	leftDriveCurrent = UART.data.avgMotorCurrent;
	leftDriveInputCurrent = UART.data.avgInputCurrent;

	// Poll the other drive VESC
	UART.getVescValues(3); 
	rightDriveRPM = UART.data.rpm;
	rightDriveCurrent = UART.data.avgMotorCurrent;
	rightDriveInputCurrent = UART.data.avgInputCurrent;

	// LED Code First Version
	FastLED.setBrightness(50);

if ( weaponCalcRPM >= 400) {
		leds[0] = CRGB::Green;
		leds[1] = CRGB::Green;
		leds[2] = CRGB::Green;
		FastLED.show();
	} else {
		leds[0] = CRGB::Red;
		leds[1] = CRGB::Red;
		leds[2] = CRGB::Red;
		FastLED.show();
	}

  if ( UART.getImuData() ) {

	Serial.print(">VESCimuRoll:");
	Serial.println(UART.data.imuRoll*180/ PI);
	Serial.print(">VESCimuPitch:");
	Serial.println(UART.data.imuPitch*180/ PI);
	Serial.print(">VESCimuYaw:");
	Serial.println(UART.data.imuYaw*180/ PI);
	Serial.print(">VESCimuAccX:");
	Serial.println(UART.data.accX);
	Serial.print(">VESCimuAccY:");
	Serial.println(UART.data.accY);
	Serial.print(">VESCimuAccZ:");
	Serial.println(UART.data.accZ);
	// Serial.print(">VESCimuGyroX:");
	// Serial.println(UART.data.gyroX);
	// Serial.print(">VESCimuGyroY:");
	// Serial.println(UART.data.gyroY);
	// Serial.print(">VESCimuGyroZ:");
	// Serial.println(UART.data.gyroZ);
\
  }
  else
  {
    Serial.println("Failed to get data!");
  }
	// delay before next reading:
	// delay(10);


	//LED Testing
	//    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    //   // Turn our current led on to white, then show the leds
    //   leds[whiteLed] = CRGB::White;
 
    //   // Show the leds (only one of which is set to white, from above)
    //   FastLED.show();
 
    //   // Wait a little bit
    //   delay(100);
 
    //   // Turn our current led back to black for the next loop around
    //   leds[whiteLed] = CRGB::Black;

}