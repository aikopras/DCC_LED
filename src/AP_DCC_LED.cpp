//******************************************************************************************************
//
// file:      AP_DCC_LED.cpp
// author:    Aiko Pras
// history:   2019-02-24 V1.0.2 ap initial version, changed into Arduino library
//            2021-06-15 V1.1 ap: attach/detach for pins (instead of constructor)
//
// purpose:   Functions related to LEDs
//
//******************************************************************************************************
#include <Arduino.h>
#include "AP_DCC_LED.h"


void DCC_Led::attach(uint8_t pin){
  _pin = pin;
  pinMode(_pin, OUTPUT);
  mode = alwaysOff;
  last_flash_time = millis();
  // default values for fading
  fadeTime = 40;                  // In 100ms steps
  fadeSteps = 50;                 // Number of steps between LED is 100% and 0%
  pwmFrequency = 50;              // PWM frequency in Herz (preferably 50 or higher)
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
  flashOntime = 2;                // 0,2 sec
  flashCount = 1;                 // single flash
  mode = singleFlashSerie;
  flash_time_remain = flashOntime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);
}


// Single short flash, to indicate a RS-Bus feedback
void DCC_Led::feedback(void) {
  flashOntime = 5;                // 0,5 sec
  flashCount = 1;                 // single flash
  mode = singleFlashSerie;
  flash_time_remain = flashOntime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);
}


// Two short flashes, to indicate decoder start
void DCC_Led::start_up(void) { 
  flashOntime = 2;                // 0,2 sec
  flashOfftime = 2;               // 0,2 sec
  flashCount = 2;                 // 2 flashes
  mode = singleFlashSerie;
  flash_time_remain = flashOntime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);
}


void DCC_Led::flashSlow(void) {
  flashOntime = 5;                // 0,5 sec
  flashOfftime = 5;               // 0,5 sec
  flashCount = 1;                 // 1 flashes
  flashPause = 5;
  mode = neverStopFlashing;
  flash_time_remain = flashOntime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);  
}


void DCC_Led::flashFast(void) {
  flashOntime = 1;                // 0,1 sec
  flashOfftime = 2;               // 0,2 sec
  flashCount = 1;                 // 1 flashes
  flashPause = 2;
  mode = neverStopFlashing;
  flash_time_remain = flashOntime;
  flash_number_now = 1;
  digitalWrite(_pin, HIGH);  
}


//******************************************************************************************************
// make a series of flashes, then a longer pause
void DCC_Led::flash(void) {
  flash_time_remain = flashOntime;
  flash_number_now = 1;                          // This is the first flash
  digitalWrite(_pin, HIGH);
}


//******************************************************************************************************
// The LED should fade out
void DCC_Led::fadeOut(void) {
  fadeLedIsOn = false;
  brightnessLevel = fadeSteps;                   // To dim the LED brightness level counts down
  fadeStepTime = 100000 / fadeSteps * fadeTime;
  pwmInterval = 1000000 / pwmFrequency;
  pwmOnTime = pwmInterval / 100 * brightnessLevel ;
  pwmOffTime = pwmInterval - pwmOnTime;
  mode = fadingOut;
}


//******************************************************************************************************
// Called from update() if our mode is singleFlashSerie or neverStopFlashing
void DCC_Led::update_flash(void) {
  unsigned long current_time = millis();         // Seems we do a single or continuous series of flashes
  if ((current_time - last_flash_time) >= 100) { // We only update the LED every 100 msec
    last_flash_time = current_time;
    --flash_time_remain;                         // Another 100 msec passed
    if (flash_time_remain == 0) {                // Do we need to change state?
      if (digitalRead(_pin)) {                   // LED is currently ON (AVR Pin for LED is high)
        digitalWrite(_pin, LOW);                 // Change is needed, thus change LED to OFF
        if (flash_number_now != flashCount)      // We did not yet had all flashes of the series
          flash_time_remain = flashOfftime;      // So we will wait a normal off time
        else {                                   // We had the complete series of flashes
          if (mode == neverStopFlashing) {       // We have to start a new series of flashes
            flash_time_remain = flashPause;      // Next we will wait a longer pause
            flash_number_now = 0;                // Restart the counter for the required number of flashes
          }
          else mode = alwaysOff;                 // We don't need to start a new series of flashes
        }
      }
      else {                                     // LED is OFF
        digitalWrite(_pin, HIGH);                // Change is needed, thus change LED to ON
        flash_time_remain = flashOntime;         // Next will be a certain time on
        flash_number_now++;                      // Increment the number of blinks we did
      }
    }
  }
}


//******************************************************************************************************
// Called from update() if our mode is singleFlashSerie or neverStopFlashing
void DCC_Led::update_fade_out(void) {
  // Is it time to lower the LED's brightness?
  unsigned long Fade_Interval = micros() - last_fade_time;
  if (Fade_Interval > (fadeStepTime)) {
    if (brightnessLevel >= 1) brightnessLevel--;
    if (brightnessLevel == 0) mode = alwaysOff;
    pwmOnTime = pwmInterval / 100 * brightnessLevel ;
    pwmOffTime = pwmInterval - pwmOnTime;
    last_fade_time = micros();
  }
  // Below the code for PWM
  unsigned long PWM_Interval = micros() - last_pwm_time;
  if (fadeLedIsOn) {
    if (PWM_Interval > pwmOnTime) {       // pwmOnTime is over: LED has been on long enough
      last_pwm_time = micros();;
      digitalWrite(_pin, LOW);
      fadeLedIsOn = false;
    }
  }
  else {
    if (PWM_Interval > pwmOffTime) {      // pwmOffTime is over: LED has been off long enough
      last_pwm_time = micros();;
      digitalWrite(_pin, HIGH);
      fadeLedIsOn = true;
    }
  }
}


//******************************************************************************************************
// update() will be called from main as often as possible
//******************************************************************************************************
void DCC_Led::update(void) {
  switch (mode) {
    case alwaysOn:                               // If the LED must be always ON,  no updating needed
      break;
    case alwaysOff:                              // If the LED must be always OFF, no updating needed
      break;
    case singleFlashSerie:                       // Handle flashes
    case neverStopFlashing:
      update_flash();
      break;
    case fadingOut:
      update_fade_out();
      break;
  }
}

