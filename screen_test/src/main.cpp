// This code renders a 3D cube, accepting inputs to do some visual changes

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "Cube.hpp"
#include "Inputs.hpp"

//      --- Constants ---

#define WIDTH 128
#define HEIGHT 64

//      --- Globals ---

// Not sure if I must call it "screen" or "display" tbh. Ok I looked up, it's display.
Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, -1);

//      --- Program ---

void setup() {
  pinMode(PIN_POTENTIOMETER, INPUT);

  pinMode(PIN_BUTTON_INVERT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SOLID, INPUT_PULLUP);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  
    // I added this because sometimes the screen just stayed white.
    // It happened when I changed 'invert' from 'y' to 'n'
    display.clearDisplay();
    display.display();
}

void loop() {
  handleInputs(distance, invert, solid);
  
  updateCube();
  renderCube(display);

  delay(5);
}
