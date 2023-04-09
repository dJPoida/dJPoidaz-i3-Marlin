#include "AnalogPin.h"

// Comment out this line to remove serial debug
// The actual responsiveness of the fan will not be accurate until serial is disabled
//#define ENABLE_SERIAL_DEBUG

int pinMosfet = 5;
AnalogPin pinSpeedPot(A0);

int speedPotReading = 0;
float fanSpeed = 0;

void setup() {
  // Set Hardware PWM Frequency for D9 & D10
  // TCCR1B = TCCR1B & B11111000 | B00000101; // for PWM frequency of 30.64 Hz
  // TCCR1B = TCCR1B & B11111000 | B00000100; // for PWM frequency of 122.55 Hz
  // TCCR1B = TCCR1B & B11111000 | B00000011; // for PWM frequency of 490.20 Hz (The DEFAULT)
  // TCCR1B = TCCR1B & B11111000 | B00000010; // for PWM frequency of 3921.16 Hz
  // TCCR1B = TCCR1B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz

  // Set Hardware PWM Frequency for D5 & D6
  // TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500.00 Hz
  // TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz
  // TCCR0B = TCCR0B & B11111000 | B00000011; // for PWM frequency of 976.56 Hz (The DEFAULT)
  // TCCR0B = TCCR0B & B11111000 | B00000100; // for PWM frequency of 244.14 Hz
  TCCR0B = TCCR0B & B11111000 | B00000101; // for PWM frequency of 61.04 Hz

  // Setup the pins
  pinSpeedPot.setNoiseThreshold(10);
  pinSpeedPot.setSmoothWeight(16);
  pinMode(pinMosfet, OUTPUT);

  // Setup Serial Monitor
  #ifdef ENABLE_SERIAL_DEBUG
  Serial.begin(115200);
  #endif
}

void loop() {
  // Read the speed input potentiometer
  speedPotReading = pinSpeedPot.readSmoothed();

  // Ensure we don't try to drive the motor too low
  if (speedPotReading < 10) {
    speedPotReading = 0;
  } else if (speedPotReading > 1013) {
    speedPotReading = 1023;
  }

  // Convert the input reading to a speed value
  fanSpeed = map(speedPotReading, 0, 1023, 0, 100);
  analogWrite(pinMosfet, map(fanSpeed, 0, 100, 0, 255)); 
  
  #ifdef ENABLE_SERIAL_DEBUG
  Serial.println(fanSpeed);
  #endif
}
