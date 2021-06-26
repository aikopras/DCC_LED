//******************************************************************************************************
//
// Test sketch for the AP_DCC_LED library
// The onboard LED should slowly fade out
// 2021/06/26 AP
//
//******************************************************************************************************
#include <Arduino.h>
#include <AP_DCC_LED.h>

const uint8_t ledPin = 13;           // Pin the LED is connected to
DCC_Led onBoardLed;                  // Instantiate the LED object


void setup() {
  onBoardLed.attach(ledPin);

  // Set the parameters that control fading
  onBoardLed.fadeTime = 50;          // 50 * 100ms (all times are in 100ms steps)
  // The default values for fadeSteps and pwmFrequency are 50, but may be modified
  onBoardLed.fadeSteps = 100;        // Number of steps between LED is 100% and 0%
  onBoardLed.pwmFrequency = 100;     // PWM frequency in Herz (preferably 50 or higher)

  // Initiate fading Out
  onBoardLed.fadeOut();
}


void loop() {
  onBoardLed.update();
}
