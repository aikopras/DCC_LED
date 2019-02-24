# DCC_LED #

Arduino library containing all the LED specific code needed for the various DCC decoders.

Any series of LED flashes can be configured by specifying the time the LED is on, the time the LED is off, the number of flashes in a single series of flashes and the pause between series of flashes, provided that flashing needs to be continuous. Some predefined settings are available for tasks such as start up, show that there is some activity, feedback, slow flashing and fast flashing.

If the LED is put in flashing mode, the following attributes  can be set (all times are in steps of 100 msec.):
- ontime:  (0..255) time the LED is on (in 100 msec steps)
- offtime: (0..255) time between to flashes
- count: (0..255) the total number of flashes (within a single serie)
- mode:  (singleFlashSerie, neverStopFlashing) whether we have a single series of flashes, or repeating_
- pause: (0..255) time between two series of flashes (in case of continuous flashing)_


# Details #
    class DCC_Led {
      public:
        uint8_t ontime;
        uint8_t offtime;
        uint8_t count;
        uint8_t pause;

        enum { alwaysOn, alwaysOff, singleFlashSerie, neverStopFlashing} mode;
        
        DCC_Led(uint8_t pin);
        // Basic functions: on, off and flash
        void turn_on(void); 
        void turn_off(void);
        void flash(void);
        void start_up(void);
        
        // Some functions for common tasks with preset attributes_
        void activity(void);
        void feedback(void);
        void flashSlow(void);
        void flashFast(void);
        
        // Should be called from the main loop as often as possible
        void update(void);
    }
