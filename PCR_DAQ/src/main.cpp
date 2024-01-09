#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

  // Define IO Ports
    //IMU
    //SD Card
    //LED TAPE

  // Define Interfaces for Each controller we are polling data from

}
// My comment
void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}