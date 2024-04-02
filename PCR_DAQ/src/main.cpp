#include <Arduino.h>
#include <SD.h>
#include <VescUart.h>
#include <FastLED.h>
#include <TimeLib.h>

// Accelerometer declarations
// #define xpin = A4;
// #define ypin = A5;
// #define zpin = A6;

// LED declarations
#define DATA_PIN 3
#define NUM_LEDS 10
CRGB leds[NUM_LEDS];


// Motor/ESC Declarations
const int poles = 16;

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
// Local connection is to ID 11 (Drive VESC)
// Second Drive is on ID 12
// Weapon is on ID 21 and 22

const int chipSelect = BUILTIN_SDCARD;

// RTC Period Read Fucntion
uint64_t get_RTC_periods()
{
    uint32_t hi1 = SNVS_HPRTCMR, lo1 = SNVS_HPRTCLR;   
    while (true)
    {
        uint32_t hi2 = SNVS_HPRTCMR, lo2 = SNVS_HPRTCLR; 
        if (lo1 == lo2 && hi1 == hi2)
        {
            return (uint64_t)hi2 << 32 | lo2;
        }
        hi1 = hi2;
        lo1 = lo2;
    }
}

#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     return pctime;
     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
       pctime = 0L; // return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void printAllVESCData(File dataFile){


		Serial.println("This the ");
		Serial.print(">VESCID:");
		Serial.println(UART.data.id);
		dataFile.println(UART.data.id);

		Serial.print(">VESCinpVolt:");
		Serial.println(UART.data.inpVoltage);
		dataFile.println(UART.data.inpVoltage);


		// Serial.println(UART.data.ampHours);
		// Serial.print(">VESCOOdometer:");
		// Serial.println(UART.data.tachometer);
		// Serial.print(">VESCRPM:");
		// Serial.println(UART.data.rpm);
		// Serial.print(">VESCcalcRPM:");
		calcRPM = (UART.data.rpm) / (poles / 2);
		// Serial.println(calcRPM);
		// Serial.print(">VESCTemp:");
		// Serial.println(UART.data.tempMosfet);

	
		// Serial.print(">VESCimuMask:");
		// Serial.println(UART.data.imuMask);
		// dataFile.println(UART.data.imuMask);

		Serial.print(">VESCimuRoll:");
		Serial.println(UART.data.imuRoll*180/ PI);
		dataFile.println(UART.data.imuRoll*180/ PI);

		Serial.print(">VESCimuPitch:");
		Serial.println(UART.data.imuPitch*180/ PI);
		dataFile.println(UART.data.imuPitch*180/ PI);

		Serial.print(">VESCimuYaw:");
		Serial.println(UART.data.imuYaw*180/ PI);
		dataFile.println(UART.data.imuYaw*180/ PI);

		Serial.print(">VESCimuAccX:");
		Serial.println(UART.data.accX);
		dataFile.println(UART.data.accX);

		Serial.print(">VESCimuAccY:");
		Serial.println(UART.data.accY);
		dataFile.println(UART.data.accY);

		Serial.print(">VESCimuAccZ:");
		Serial.println(UART.data.accZ);
		dataFile.println(UART.data.accZ);

		Serial.print(">VESCimuGyroX:");
		Serial.println(UART.data.gyroX);
		dataFile.println(UART.data.gyroX);

		Serial.print(">VESCimuGyroY:");
		Serial.println(UART.data.gyroY);
		dataFile.println(UART.data.gyroY);

		Serial.print(">VESCimuGyroZ:");
		Serial.println(UART.data.gyroZ);
		dataFile.println(UART.data.gyroZ);

		// Serial.print(">VESCimuMagX:");
		// Serial.println(UART.data.magX);
		// dataFile.println(UART.data.magX);

		// Serial.print(">VESCimuMagY:");
		// Serial.println(UART.data.magY);
		// dataFile.println(UART.data.magY);

		// Serial.print(">VESCimuMagZ:");
		// Serial.println(UART.data.magZ);
		// dataFile.println(UART.data.magZ);

		// Serial.print(">VESCimuQ0:");
		// Serial.println(UART.data.q0);
		// dataFile.println(UART.data.q0);

		// Serial.print(">VESCimuQ1:");
		// Serial.println(UART.data.q1);
		// dataFile.println(UART.data.q1);

		// Serial.print(">VESCimuQ2:");
		// Serial.println(UART.data.q2);
		// dataFile.println(UART.data.q2);

		// Serial.print(">VESCimuQ3:");
		// Serial.println(UART.data.q3);
		// dataFile.println(UART.data.q3);

	}

void setup() {
	// Initialize the serial communications:
	Serial.begin(115200);

	//Enable the led
	FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

	//Enable Serial on a port for UART
	Serial5.begin(115200);

	// Set the port for the UART communication to the VESC
	UART.setSerialPort(&Serial5);
	//UART.setDebugPort(&Serial);

	Serial.print("Initializing SD card...");

  	if (!SD.begin(chipSelect)) {
    	Serial.println("initialization failed!");
    	return;
  	}
  	Serial.println("initialization done.");

	//Initialize the RTC Sync
	setSyncProvider(getTeensy3Time);   // the function to get the time from the RTC
  	if (timeStatus() != timeSet) 
		Serial.println("Unable to sync with the RTC");
	else
		Serial.println("RTC has set the system time");      

}

void loop() {
	// Real Time Clock Values
	uint64_t periods      = get_RTC_periods();
    time_t seconds        = periods / 32768;
    uint32_t milliseconds = (1000 * (periods & 0x7FFF)) / 32768;

    char buf[80];
    tm* timeinfo = localtime(&seconds);
    strftime(buf, 80, "%F_%H:%M:%S", timeinfo);
    // Serial.sprintf("%s.%03d\n", buf, milliseconds); // CHANGE THIS

	// open data file
	File dataFile = SD.open("data.txt", FILE_WRITE);
	// if(SD.exists("data.txt")){
	// 	Serial.println("Yippy! It exists!");
	// }
	// else{
	// 	Serial.println("NO! IT DOES NOT EXIST!");
	// }

	Serial.println("\nNew Data Set\n");
	dataFile.println("\nNew Data Set\n");
	dataFile.printf("%s.%03d\n", buf, milliseconds);

	// print the sensor values:
	// Serial.print(">xaxis:");
	// Serial.println(analogRead(xpin));
	// print a tab between values:
	// Serial.print("\n");
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

	// weaponValueIn = analogRead(weaponCurrent);
	// weaponValueOut = map(weaponValueIn, 0, 1023, 0, 255);


	if (Serial.available()) {
		time_t t = processSyncMessage();
		if (t != 0) {
			Teensy3Clock.set(t); // set the RTC
			setTime(t);
			}
	}
	// Poll the directly attached VESC for data (Drive 1 VESC)
	UART.getVescValues();
	UART.getImuData();
	printAllVESCData(dataFile);

	// Poll the drive VESC for data (CAN ID of Drive 2 VESC)
	UART.getVescValues(12);
	UART.getImuData(12);
	printAllVESCData(dataFile);

	// Poll the Weapon VESC for data (CAN ID of Weapon 1 VESC)
	UART.getVescValues(21);
	UART.getImuData(21);
	printAllVESCData(dataFile);
	weaponRPM = UART.data.rpm;
	calcRPM = (UART.data.rpm) / (poles / 2);
	// weaponCurrent = UART.data.avgMotorCurrent;
	// weaponInputCurrent = UART.data.avgInputCurrent;

	// Poll the Weapon VESC for data (CAN ID of Weapon 2 VESC)
	UART.getVescValues(22);
	UART.getImuData(22);
	printAllVESCData(dataFile);


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
	dataFile.close();

	// delay before next reading:
	delay(100);


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