// This library renders a rotating 3d cube

#ifndef CUBE_H
#define CUBE_H

#include <Adafruit_SSD1306.h>

//          --- Public Variables ---

extern bool invert;
extern bool solid;
extern int distance;

//          --- Public Functions ---

// So... I just noticed that Arduino's functions are all in camelCase. 
// Let's keep the style.

void updateCube();
void renderCube(Adafruit_SSD1306 &display);

#endif
