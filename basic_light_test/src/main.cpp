// This program controls a blinking led.

#include <Arduino.h>

//      --- Contants ---

const int PIN_LED = 18;

//      --- Globals ---

int blinking_delay = 1000;

//      --- Function Declarations ---

void blink(int pin, int blinking_delay);

//      --- Program ---

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  blink(PIN_LED, blinking_delay);
}

//      --- Functions ---

void blink(int pin, int blinking_delay) {
  digitalWrite(pin, HIGH);

  delay(blinking_delay);

  digitalWrite(pin, LOW);

  delay(blinking_delay);
}
