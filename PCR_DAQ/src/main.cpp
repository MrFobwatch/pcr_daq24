#include <Arduino.h>
#include <VescUart.h>
#include <FastLED.h>

// Accelerometer declarations
#define xpin = A4;
#define ypin = A5;
#define zpin = A6;

// LED declarations
const int LED = 7;
#define DATA_PIN 3
#define NUM_LEDS 10
CRGB leds[NUM_LEDS];


int ledState = LOW;

// Motor/ESC Declarations
const int poles = 14;

// Weapon Variables
// const int weaponCurrent = A0;
int weaponValueIn = 0;
int weaponValueOut = 0;

long weaponRPM = 0;
float weaponCurrent = 0;
float weaponInputCurrent = 0;
int calcRPM;

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

	//Enable the led
	FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

	//Enable Serial on a port for UART
	Serial1.begin(115200);

	while (!Serial1) {;}

	// Set the port for the UART communication to the VESC
	UART.setSerialPort(&Serial1);
	UART.setDebugPort(&Serial);
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

	weaponRPM = UART.data.rpm;
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

	FastLED.setBrightness(50);

if ( calcRPM >= 400) {
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
	//Test UART connection
UART.printVescValues();
if ( UART.getVescValues() ) {

    // Serial.println(UART.data.rpm);
	Serial.print(">VESCinpVolt:");
	Serial.println(UART.data.inpVoltage);
    // Serial.println(UART.data.ampHours);
	// Serial.print(">VESCOOdometer:");
    // Serial.println(UART.data.tachometer);
	Serial.print(">VESCRPM:");
    Serial.println(UART.data.rpm);
	Serial.print(">VESCcalcRPM:");
	calcRPM = (UART.data.rpm) / (poles / 2);
	Serial.println(calcRPM);
	Serial.print(">VESCTemp:");
	Serial.println(UART.data.tempMosfet);

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