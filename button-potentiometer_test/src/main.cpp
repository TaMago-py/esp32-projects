#include <Arduino.h>

int pin_button_r = 27;
int pin_button_g = 33;
int pin_button_b = 32;

int pin_led_r = 18;
int pin_led_g = 19;
int pin_led_b = 23;

int pin_potentiometer = 35;

int last_r;
int last_g;
int last_b;

void setup() {

    pinMode(pin_button_r, INPUT_PULLUP);
    pinMode(pin_button_g, INPUT_PULLUP);
    pinMode(pin_button_b, INPUT_PULLUP);

    pinMode(pin_led_r, OUTPUT);
    pinMode(pin_led_g, OUTPUT);
    pinMode(pin_led_b, OUTPUT);

    pinMode(pin_potentiometer, INPUT);
}

void loop() {

    int potentiometer_read = analogRead(pin_potentiometer);

    // I read that the potentiometer goes from 0 to 4095, so I had to map it
    // to work with the leds.
    int led_level = map(potentiometer_read, 0, 4095, 0, 255);

    int button_r = digitalRead(pin_button_r);
    int button_g = digitalRead(pin_button_g);
    int button_b = digitalRead(pin_button_b);

    // For every color I save its last level for it to not change with the others.
    if (button_r == LOW) {

        last_r = led_level;

        analogWrite(pin_led_r, last_r);
    }

    if (button_g == LOW) {

        last_g = led_level;

        analogWrite(pin_led_g, last_g);
    }

    if (button_b == LOW) {

        last_b = led_level;

        analogWrite(pin_led_b, last_b);
    }

    delay(10);
}