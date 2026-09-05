#include <Arduino.h>

const int pin_red = 18;
const int pin_green = 19;
const int pin_blue = 23;

// starts with a pure red light
int rgb[3] = {255, 0, 0};

int range = 0;

void update_color(int rgb[3]);

void setup() {

  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);
}

void loop() {

  // Here I separate the color change by sections
  switch (range) {

    // From red to green.
    case 0:
      rgb[1]++;
      rgb[0]--;

      // new section/range. I'm not sure about how to call it.
      if (rgb[1] == 255) range = 1;
      break;

      // From green to blue.
    case 1:
      rgb[2]++;
      rgb[1]--;
      if (rgb[2] == 255) range = 2;
      break;

      // And from blue to red.
    case 2:
      rgb[0]++;
      rgb[2]--;
      if (rgb[0] == 255) range = 0;
      break;
  }

  update_color(rgb);

  delay(10);
}

void update_color(int rgb[3]) {

  // changes every color by it's value in the rgb array.
  analogWrite(pin_red, rgb[0]);
  analogWrite(pin_green, rgb[1]);
  analogWrite(pin_blue, rgb[2]);
}