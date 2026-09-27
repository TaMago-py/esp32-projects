// This library manages the inputs for the program

#include <Arduino.h>
#include "Inputs.hpp"

//      --- Globals ---

int last_button_state_invert = HIGH;
int last_button_state_solid = HIGH;

//      --- Private Function Declarations ---

bool checkButtons(int pin, int &last_button_state);

//      --- Public Functions ---

/**
 * @brief Handles the inputs from the potentiometer and the buttons
 * 
 * @param distance Cube's library 'distance' variable
 * @param invert Cube's library 'invert' variable
 * @param solid Cube's library 'solid' variable
 */
void handleInputs(int &distance, bool &invert, bool &solid) {
  int potentiometer_value = analogRead(PIN_POTENTIOMETER);
  
  // I think 50 to 250 is a good range for the cube's distance. If its too low it
  // suffers from distortion, and if its too high, well, its too far to the screen to see.
  distance = map(potentiometer_value, 0, 4095, 50, 250);

  if (checkButtons(PIN_BUTTON_INVERT, last_button_state_invert)) {
    invert = !invert;
  }

  if (checkButtons(PIN_BUTTON_SOLID, last_button_state_solid)) {
    solid = !solid;
  }
}

//      --- Private Functions ---

/**
 * @brief Manages a button toggle system by checking its current and last state
 * 
 * @param pin The button's pin
 * @param last_button_state The button's last state (HIGH or LOW)
 * 
 * @return pressed, being this a bool variable that returns 
 * true if the button was toggle pressed
 */
bool checkButtons(int pin, int &last_button_state) {
  int current_button_state = digitalRead(pin);

  // Only true if it was previously HIGH, and is now LOW. As the last state changes immediately after the button
  // is pressed, the condition can not be true until you unpress the button and press it again.
  bool pressed = (last_button_state == HIGH && current_button_state == LOW);
  
  last_button_state = current_button_state;

  return pressed;
}
