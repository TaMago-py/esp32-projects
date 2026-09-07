#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define WIDTH 128
#define HEIGHT 64

bool visible(int vertex_0, int vertex_1, int vertex_2);

const int pin_button_invert = 18;
const int pin_button_solid = 19;

const int pin_potentiometer = 35;

int button_state_invert;
int last_button_state_invert;

int button_state_solid;
int last_button_state_solid;

// Not sure if I must call it "screen" or "display" tbh. ok I looked up, it's display.
Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, -1);

// vertices of a cube.
const float vertices[8][3] = {
  {-20, -20, -20}, {20, -20, -20}, {20, 20, -20}, {-20, 20, -20}, // Front face
  {-20, -20, 20}, {20, -20, 20}, {20, 20, 20}, {-20, 20, 20} // Back face
};

int vertices2D[8][2];

// The faces of the cube drawn clockwise. it's clockwise, because in the screen, the 'y'
// axis is positive going down. also, the 'z' axis is positive when it's far the screen.
const int faces[6][4] {
  {0, 1, 2, 3},
  {1, 5, 6, 2},
  {5, 4, 7, 6},
  {4, 0, 3, 7},
  {3, 2, 6, 7},
  {4, 5, 1, 0},
};

float angle_x = 0.0;
float angle_y = 0.0;

// 'true' for white screen, 'false' for black screen.
bool invert = false;

// "true" for a faced cube, 'false' for just the edges.
bool solid = false;

int distance;

void setup() {

  pinMode(pin_potentiometer, INPUT);

  pinMode(pin_button_invert, INPUT_PULLUP);
  pinMode(pin_button_solid, INPUT_PULLUP);

  button_state_invert = digitalRead(pin_button_invert);
  button_state_solid = digitalRead(pin_button_solid);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    
    for(;;);
  }
    
    // I added this because sometimes the screen just stayed white.
    // It happened when I changed 'invert' from 'y' to 'n'
    display.clearDisplay();
    display.display();
}

void loop() {

  display.invertDisplay(invert);
  
  display.clearDisplay();
  
  int potentiometer_value = analogRead(pin_potentiometer);
  distance = map(potentiometer_value, 0, 4095, 50, 250);
  
  last_button_state_invert = button_state_invert;
  last_button_state_solid = button_state_solid;

  button_state_invert = digitalRead(pin_button_invert);
  button_state_solid = digitalRead(pin_button_solid);

  if (last_button_state_invert == HIGH && button_state_invert == LOW) {

    invert = !invert;
  }

  if (last_button_state_solid == HIGH && button_state_solid == LOW) {

    solid = !solid;
  }

  for (int i = 0; i < 8; i++) {

    // The vectors in a matrix goes [x y z]
    float x = vertices[i][0];
    float y = vertices[i][1];
    float z = vertices[i][2];

    // Rotating around the 'y' axis.
    float x1 = x * cos(angle_y) - z * sin(angle_y);
    //    y1 = y
    float z1 = x * sin(angle_y) + z * cos(angle_y);

    // Rotating around the 'x' axis
    // x2 = x1
    float y2 = y * cos(angle_x) - z1 * sin(angle_x);
    float z2 = y * sin(angle_x) + z1 * cos(angle_x);

    // the 3D vectors projection into 2D.
    vertices2D[i][0] = (int)(x1 * 60 / (z2 + distance)) + 64;
    vertices2D[i][1] = (int)(y2 * 60 / (z2 + distance)) + 32;
  }
  
  if (solid) {
    
    for (int j = 0; j < 6; j++) {
      
      // Takes the vertices forming the actual face.
      int p0 = faces[j][0];
      int p1 = faces[j][1];
      int p2 = faces[j][2];
      int p3 = faces[j][3];
      
      if (visible(p0, p1, p2)) {
        
        // Draws the face if it's visible.
        display.drawLine(vertices2D[p0][0], vertices2D[p0][1], vertices2D[p1][0], vertices2D[p1][1], WHITE);
        display.drawLine(vertices2D[p1][0], vertices2D[p1][1], vertices2D[p2][0], vertices2D[p2][1], WHITE);
        display.drawLine(vertices2D[p2][0], vertices2D[p2][1], vertices2D[p3][0], vertices2D[p3][1], WHITE);
        display.drawLine(vertices2D[p3][0], vertices2D[p3][1], vertices2D[p0][0], vertices2D[p0][1], WHITE);
      }
    }
  }
  
  else {
    
    for (int i = 0; i < 4; i++) {
  
    // Drawing every edge of the box.
    // I want to try making some other geometrical figures too.
    display.drawLine(vertices2D[i][0], vertices2D[i][1], vertices2D[(i + 1) % 4][0], vertices2D[(i + 1) % 4][1], WHITE);
    display.drawLine(vertices2D[i + 4][0], vertices2D[i + 4][1], vertices2D[((i + 1) % 4) + 4][0], vertices2D[((i + 1) % 4) + 4][1], WHITE);
    display.drawLine(vertices2D[i][0], vertices2D[i][1], vertices2D[i + 4][0], vertices2D[i + 4][1], WHITE);
    }
  }
  
  display.display();
  
  // Modifying the angles. don't know why, but if both are the same value the animation
  // comes out laggy in a point. More like a little jump.
  angle_x += 0.03;
  angle_y += 0.02;

  delay(5);
}

bool visible(int vertex_0, int vertex_1, int vertex_2) {

  // I take three consecutive vertex, and I made the vectors from the first one
  // to the other two. 
  int x1 = vertices2D[vertex_1][0] - vertices2D[vertex_0][0];
  int y1 = vertices2D[vertex_1][1] - vertices2D[vertex_0][1];

  int x2 = vertices2D[vertex_2][0] - vertices2D[vertex_0][0];
  int y2 = vertices2D[vertex_2][1] - vertices2D[vertex_0][1];

  // if it's positive, it means the face is visible by going clockwise, else it's
  // not visible, by going counter-clockwise.
  long cross_point = x1 * y2 - x2 * y1;

  return cross_point >= 0;
}