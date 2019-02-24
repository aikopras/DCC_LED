//******************************************************************************************************
//
// file:      DCC_LED.cpp
// author:    Aiko Pras
// history:   2019-02-24 V1.0.2 ap initial version, changed into Arduino library
//
// purpose:   Functions related to LEDs
//
//******************************************************************************************************
#include <Arduino.h>
#include "DCC_LED.h"


DCC_Led::DCC_Led(uint8_t pin){
  _pin = pin;
  pinMode(_pin, OUTPUT);
  mode = alwaysOff;
  last_time = millis();
}


void DCC_Led::turn_on(void){
  digitalWrite(_pin, HIGH);
  mode = alwaysOn;
}


void DCC_Led::turn_off(void){
  digitalWrite(_pin, LOW);
  mode = alwaysOff;
}


//******************************************************************************************************
// Single very short flash, to indicate a switch command
void DCC_Led::activity(void) {
  ontime = 2;                     // 0,2 sec
  count = 1;                      // single flash
  mode = singleFlashSerie;
  time_remain = ontime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);
}


// Single short flash, to indicate a RS-Bus feedback
void DCC_Led::feedback(void) {
  ontime = 5;                     // 0,5 sec
  count = 1;                      // single flash
  mode = singleFlashSerie;
  time_remain = ontime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);
}


// Two short flashes, to indicate decoder start
void DCC_Led::start_up(void) { 
  ontime = 2;                     // 0,2 sec
  offtime = 2;                    // 0,2 sec
  count = 2;                      // 2 flashes
  mode = singleFlashSerie;
  time_remain = ontime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);
}


void DCC_Led::flashSlow(void) {
  ontime = 5;                     // 0,3 sec
  offtime = 5;                    // 0,3 sec
  count = 1;                      // 1 flashes
  pause = 5;
  mode = neverStopFlashing;
  time_remain = ontime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);  
}


void DCC_Led::flashFast(void) {
  ontime = 1;                     // 0,3 sec
  offtime = 2;                    // 0,3 sec
  count = 1;                      // 1 flashes
  pause = 2;
  mode = neverStopFlashing;
  time_remain = ontime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);  
}


//******************************************************************************************************
// make a series of flashes, then a longer pause
void DCC_Led::flash(void) {
  time_remain = ontime;
  flash_number_now = 1;                       // This is the first flash
  digitalWrite(_pin, HIGH);
}


//******************************************************************************************************
// update() will be called from main as often as possible
//******************************************************************************************************
void DCC_Led::update(void) {
  if (mode == alwaysOn)  return;              // If the LED must be always ON,  no updating needed 
  if (mode == alwaysOff) return;              // If the LED must be always OFF, no updating needed 
  unsigned long current_time = millis();      // Seems we do a series of flashes
  if ((current_time - last_time) >= 100) {    // We only update the LED every 100 msec
    last_time = current_time;   
    --time_remain;                            // Another 100 msec passed
    if (time_remain == 0) {                   // Do we need to change state?
      if (digitalRead(_pin)) {                // LED is currently ON (AVR Pin for LED is high)
        digitalWrite(_pin, LOW);              // Change is needed, thus change LED to OFF 
        if (flash_number_now != count)        // We did not yet had all flashes of the series
          time_remain = offtime;              // So we will wait a normal off time
        else {                                // We had the complete series of flashes
          if (mode == neverStopFlashing) {    // We have to start a new series of flashes
            time_remain = pause;              // Next we will wait a longer pause
            flash_number_now = 0;             // Restart the counter for the required number of flashes
          }
          else mode = alwaysOff;              // We don't need to start a new series of flashes
        }
      }
      else {                                  // LED is OFF
        digitalWrite(_pin, HIGH);             // Change is needed, thus change LED to ON
        time_remain = ontime;                 // Next will be a certain time on
        flash_number_now++;                   // Increment the number of blinks we did
      }
    }
  }
}

