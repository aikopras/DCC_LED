//******************************************************************************************************
//
// file:      AP_DCC_LED.h
// author:    Aiko Pras
// history:   2019-02-24 V1.0.2 ap initial version, changed into Arduino library
//            2021-06-26 V1.1   ap extended with fade out
//
// purpose:   LED object. LED can be switched on, switched off, put in flashing mode or fade out.
//            Next to these basic modes, additional functions are defined for some common tasks,
//            such as the decoder (re)starts, performs some activity or sends feedback
//
//******************************************************************************************************
//                                               FLASHING
//******************************************************************************************************
// If the LED is put in flashing mode, the following attributes  can be set:
// - mode:         singleFlashSerie, neverStopFlashing / single or repeating series of flashes
// - flashOntime:  (0..255) time the LED is on (in 100 msec steps)
// - flashOfftime: (0..255) time between to flashes
// - flashCount:   (0..255) the total number of flashes (within a single serie)
// - flashPause:   (0..255) time between two series of flashes (in case of continuous flashing)
// All times are in steps of 100 msec.
//                                                 flashCount
//                 flashOntime
//     ----+          +----+         +----+         +----+                                    +----+
//         |          |    |         |    |         |    |                                    |    |
//         |          |    |         |    |         |    |                                    |    |
//         +----------+    +---------+    +---------+    +------------------------------------+    +----
//                        flashOfftime                                      flashPause
//
//                                                                                     mode = continuous
//
//******************************************************************************************************
//                                                 FADING
//******************************************************************************************************
//
//            +--------+
//                     +---------+
//     ^                         +---------+
//     |                                   +---------+
//     |                                             +---------+
// fadeSteps                                                     fadeTime (100ms) --->
//                               [         ]
//                              fadeStepTime
//                               (microsec)
//
//  fadeStepTime =  100000 / fadeSteps * fadeTime
//  Note1: fadeStepTime is uint16_t, so maximum is 65535. To avoid overflow, division should come first
//  Note2: We divide 100000 (instead of 1000000), since fadeTime is in 100ms (and not seconds)
//
//******************************************************************************************************
//
//                  pwmOnTime              pwmOffTime                    pwmInterval     (microsec)
//                    <--->               [           ]               [               ]
//    +---+           +---+           +---+           +---+           +---+           +
//        |           |   |           |   |           |   |           |   |           |
//        |           |   |           |   |           |   |           |   |           |
//        +-----------+   +-----------+   +-----------+   +-----------+   +-----------+
//    <------------------------------------------------------------------------------->
//    0                                                                               1 sec
//                                  pwmFrequency (Hz)
//
//  pwmInterval = 1000000 / pwmFrequency
//  pwmOnTime = pwmInterval * brightnessLevel / 100.  brightnessLevel is between 0..100)
//  pwmOffTime = pwmInterval - pwmOnTime
//
//******************************************************************************************************
//******************************************************************************************************
#pragma once

class DCC_Led {
  public:
    // Attributes to set the LED's flashing behaviour
    uint8_t flashOntime;          // For flashing: time the LED should be ON (in 100ms steps)
    uint8_t flashOfftime;         // For flashing: time the LED should be OFF (in 100ms steps)
    uint8_t flashPause;           // For flashing: LED off time between a series of flashes
    uint8_t flashCount;           // Number of flashes before a pause
    // Attributes to set the LED's fading behaviour
    uint8_t fadeTime;             // In 100ms steps
    uint8_t fadeSteps;            // Number of steps between LED is 100% and 0%
    uint8_t pwmFrequency;         // PWM frequency in Herz (preferably 50 or higher)
    // Attribute to select the LED behaviour
    enum {                        // The states the LED may be in
      alwaysOn,
      alwaysOff,
      singleFlashSerie,
      neverStopFlashing,
      fadingOut
    } mode;                    

    void attach (uint8_t pin);      // Set the pin as output pin

    // Basic functions: on, off, flash and fade
    void turn_on(void);
    void turn_off(void);
    void flash(void); 
    void fadeOut(void);

    // The functions below are for common tasks with preset attributes
    void start_up(void);
    void activity(void);
    void feedback(void);
    void flashSlow(void);           // Continuous series of slow flashes
    void flashFast(void);           // Continuous series of fast flashes
 
    void update(void);              // Schould be called from main as often as possible


  private:
    uint8_t _pin;                   // Hardware pin to which the DCC decoder LED is connected
  
    // While flashing, the following variables are used to calculate flash times
    unsigned long last_flash_time;  // time in msec since we last updated the LEDs
    uint8_t flash_number_now;       // Number of flashes thusfar
    uint8_t flash_time_remain;      // Remaining time before LED status changes. In Ticks (100ms)
  
    // While fading, the following variables are used to calculate PWM and fade times
    uint16_t fadeStepTime;          // in microseconds, see figures above
    uint16_t pwmInterval;           // in microseconds, see figures above
    uint16_t pwmOnTime;             // in microseconds, see figures above
    uint16_t pwmOffTime;            // in microseconds, see figures above
    unsigned long last_fade_time;   // time (ms) since we last updated the fade settings
    unsigned long last_pwm_time;    // time (ms) since we last updated the PWM settings
    bool fadeLedIsOn;               // State of the LED
    uint8_t brightnessLevel;        // Current LED level
  
    void update_flash(void);        // Code to handle flashes
    void update_fade_out(void);     // Code to handle fading out the LED

};
