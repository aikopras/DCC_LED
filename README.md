# AP_DCC_LED #

Arduino library containing all the LED specific code needed for the various DCC decoders. Supports flashing and fade-out.

### LED operation ###
LED operation can be selected via the following core functions:
- `turn_on()`: Turn the LED on
- `turn_off()`: Turn the LED off
- `flash()`: Put the LED in flashing mode
- `fadeOut()`: Let the LED fade out

LED operation for some common tasks can be selected using the following functions:
- `start_up()`: Two short flashes of 200ms each
- `activity()`: Single flash of 200ms
- `feedback()`: Single flash of 500ms
- `flashSlow()`: Continuous series of slow flashes
- `flashFast()`: Continuous series of fast flashes

### Flash behaviour ###
The `flash()` behaviour can be configured by specifying the time the LED is on, the time the LED is off, the number of flashes in a single series of flashes and the pause between series of flashes (if flashing needs to be continuous):
- `flashOntime`:  (0..255) time the LED is on (in 100 msec steps)
- `flashOfftime`: (0..255) time between to flashes
- `flashCount`: (0..255) the total number of flashes (within a single serie)
- `flashPause`: (0..255) time between two series of flashes

### Fade behaviour ###
The `fade()` behaviour can be configured via the following parameters:
- `fadeTime`:  (0..255) In 100ms steps. The default value is 4 seconds.
- `fadeSteps`: (0..255) Number of steps between the LED brightness being 100% and 0%. Higher values give smoother behaviour, but increase CPU load. The default is 50 steps.
- `pwmFrequency`: (0..255) The brightness of the LED is set by using a PWM (Pulse Width Modulation) signal. The default frequency is 50Hz, which is high enough for the human eye. Higher values increase CPU load.

# Examples #
#### flashing ####
````
#include <Arduino.h>
#include <AP_DCC_LED.h>

const uint8_t ledPin = 13;           // Pin the LED is connected to
DCC_Led onBoardLed;                  // Instantiate the LED object

void setup() {
  onBoardLed.attach(ledPin);
  onBoardLed.flashOntime = 2;            // 200ms
  onBoardLed.flashOfftime = 5;           // 500ms
  onBoardLed.flashCount = 5;
  onBoardLed.flashPause = 20;            // 2 seconds
  onBoardLed.mode = onBoardLed.neverStopFlashing;   
  onBoardLed.flash();                    // start flashing
}

void loop() {
  onBoardLed.update();
}
````

#### fading ####
````
#include <Arduino.h>
#include <AP_DCC_LED.h>

const uint8_t ledPin = 13;           // Pin the LED is connected to
DCC_Led onBoardLed;                  // Instantiate the LED object

void setup() {
  onBoardLed.attach(ledPin);
  onBoardLed.fadeTime = 50;          // fade out in  5 seconds
  onBoardLed.fadeOut();              // start fading out
}

void loop() {
  onBoardLed.update();
}
````
