/*
 * Index Fan PWM
 * Step through PWM speeds with momentary pushbutton to control Noctua fan speed.
 * Digispark https://content.instructables.com/ORIG/F75/EOPS/JEST5ZTO/F75EOPSJEST5ZTO.jpg
 * https://digistump.com/wiki/digispark/tricks#how_to_increase_hardware_pwm_frequency
 * Noctua PWM white paper https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf
 */
#include <Arduino.h>

/* constants won't change. They're used here to set pin numbers: */
const int buttonPin = 5;              // the number of the pushbutton pin
const int pwmPin = 0;                 // the number of the PWM pin

/* the fan speeds you want to cycle through, in any order */
const uint8_t pwmSteps[] = { 0, 64, 128, 192, 255 };

/* variables */
static int speed = 2;                 // Default starting fan speed to 50%
static uint8_t buttonFilter = 0x55;   // Debounce filter

void setup() {                        // P0, P1, and P4 are capable of hardware PWM (analogWrite).
    pinMode(pwmPin, OUTPUT);          // 0 is P0, 1 is P1, 4 is P4 - unlike the analog inputs,
                                      // for analog (PWM) outputs the pin number matches the port number.
    pinMode(buttonPin, INPUT_PULLUP); // 5 is P5
    analogWrite(pwmPin,64);           // Set the PWM pin to 25%
}

/* Loop for button press to increment PWM speed */
void loop() {
  /* use 5ms sampling; this sets the debounce to ~35ms) */
  delay(5);

  /* read the state of the switch into a local variable and shift into debounce */
  buttonFilter = (buttonFilter << 1) | digitalRead(buttonPin);

  /* if we see a rising edge, the user has released the button */
  if(buttonFilter == 0x7F) {
      /* cycle through the available modes */
    speed = (speed + 1) % sizeof(pwmSteps);
  }

  analogWrite(pwmPin,pwmSteps[speed]);
}