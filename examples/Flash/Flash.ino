//******************************************************************************************************
//
// Test sketch for the AP_DCC_LED library
// The onboard LED should blink 5 times, followed by 2 seconds off
// 2019/02/24 / 2021/06/15 AP
//
//******************************************************************************************************
#include <Arduino.h>
#include <AP_DCC_LED.h>

const uint8_t ledPin = 13;           // Pin the LED is connected to
DCC_Led onBoardLed;                  // Instantiate the LED object


void setup() {
  delay(2000);
  onBoardLed.attach(ledPin);

  onBoardLed.flashOntime = 2;            // 200ms (all times are in 100ms steps)
  onBoardLed.flashOfftime = 5;           // 500ms
  onBoardLed.flashCount = 5;
  onBoardLed.flashPause = 20;            // 2 seconds
  onBoardLed.mode = onBoardLed.neverStopFlashing;   
  onBoardLed.flash();
 
//  onBoardLed.start_up();
//  onBoardLed.activity();
//  onBoardLed.feedback();
//  onBoardLed.flashSlow();
//  onBoardLed.flashFast();
}


void loop() {
onBoardLed.update();
}
