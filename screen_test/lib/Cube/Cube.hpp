#ifndef CUBE_H
#define CUBE_H

#include <Adafruit_SSD1306.h>

//          --- Public Variables ---

extern bool invert;
extern bool solid;
extern int distance;

//          --- Public Functions ---

void updateCube();
void renderCube(Adafruit_SSD1306 &display);

#endif
