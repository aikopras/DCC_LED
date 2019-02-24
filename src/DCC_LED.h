#ifndef DCC_Decoder_LED_H
#define DCC_Decoder_LED_H

//******************************************************************************************************
//
// file:      DCC_Decoder_LED.h
// author:    Aiko Pras
// history:   2019-02-24 V1.0.2 ap initial version, changed into Arduino library
//
// purpose:   LED object. LED can be switched on, switched off or put in flashing mode.
//            Next to these three basic modes, additional functions are defined for some common
//            tasks, such as the decoder (re)starts, performs some activity or sends feedback
//
// If the LED is put in flashing mode, the following attributes  can be set:
// - ontime:  (0..255) time the LED is on (in 100 msec steps)
// - offtime: (0..255) time between to flashes 
// - count:   (0..255) the total number of flashes (within a single serie)
// - mode:    (singleFlashSerie, neverStopFlashing) whether we have a single series of flashes, or repeating  
// - pause:   (0..255) time between two series of flashes (in case of continuous flashing)
// All times are in steps of 100 msec.
//                                                     count
//                    ontime
//     ----+          +----+         +----+         +----+                                    +----+
//         |          |    |         |    |         |    |                                    |    |
//         |          |    |         |    |         |    |                                    |    |
//         +----------+    +---------+    +---------+    +------------------------------------+    +----
//                           offtime                                      pause
//
//                                                                                      mode = contnuous

//******************************************************************************************************
class DCC_Led {
  public:
    uint8_t ontime;               // SET: LED on time
    uint8_t offtime;              // SET: LED off time
    uint8_t count;                // SET: Number of flashes before a pause
    uint8_t pause;                // SET: Longer LED off time: between a series of flashes
    enum {                        // The states the LED may be in
      alwaysOn,
      alwaysOff,
      singleFlashSerie,
      neverStopFlashing
    } mode;                    

    DCC_Led(uint8_t pin);           // Constructor, to initialise the LED

    void turn_on(void);             // Basic functions: on, off and flash
    void turn_off(void);
    void flash(void); 
    
    void start_up(void);            // Some functions for common tasks with preset attributes
    void activity(void);
    void feedback(void);
    void flashSlow(void);           // Continuous series of slow flashes
    void flashFast(void);           // Continuous series of fast flashes
 
    void update(void);              // Schould be called from main as often as possible


  private:
    uint8_t _pin;                   // Hardware pin to which the DCC decoder LED is connected
    unsigned long last_time;        // time in msec since we last updated the LEDs
    uint8_t flash_number_now;       // Number of flashes thusfar
    uint8_t time_remain;            // Remaining time before LED status changes. In Ticks (100ms)

};


#endif
