#ifndef INPUTS_H
#define INPUTS_H

//          --- Public Variables ---

const int PIN_BUTTON_INVERT = 18;
const int PIN_BUTTON_SOLID = 19;

const int PIN_POTENTIOMETER = 35;

//          --- Public Functions ---

void handleInputs(int &distance, bool &invert, bool &solid);

#endif