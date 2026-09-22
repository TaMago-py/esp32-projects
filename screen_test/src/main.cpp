// This code renders a 3D rotating cube by using linear algebra.

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

//      --- Constants ---

#define WIDTH 128
#define HEIGHT 64

const int PIN_BUTTON_INVERT = 18;
const int PIN_BUTTON_SOLID = 19;

const int PIN_POTENTIOMETER = 35;

//      --- 3D Cube Data ---

// 3D cube's vertices.
const float vertices[8][3] = {
  {-20, -20, -20}, {20, -20, -20}, {20, 20, -20}, {-20, 20, -20}, // Front face
  {-20, -20, 20}, {20, -20, 20}, {20, 20, 20}, {-20, 20, 20} // Back face
};

// The faces of the cube drawn clockwise.
// Remember that on a screen the 'y' axis goes downwards.
const int faces[6][4] {
  {0, 1, 2, 3},
  {1, 5, 6, 2},
  {5, 4, 7, 6},
  {4, 0, 3, 7},
  {3, 2, 6, 7},
  {4, 5, 1, 0},
};

// Cube's vertices projection on the 2D space.
int vertices2D[8][2];

//      --- Globals ---

// Not sure if I must call it "screen" or "display" tbh. Ok I looked up, it's display.
Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, -1);

float angle_x = 0.0;
float angle_y = 0.0;

// 'true' for white screen, 'false' for black screen.
bool invert = false;

// "true" for a faced cube, 'false' for just the edges.
bool solid = false;

int last_button_state_invert;
int last_button_state_solid;

int distance = 100;

//      --- Function Declarations ---

// So... I just noticed that Arduino's function are in camelCase. Maybe I should do the same.
void handleInputs();
bool checkButtons(int pin, int &last_button_state);

void updateCube();
void renderCube();

bool isFaceVisible(int vertex_0, int vertex_1, int vertex_2);
void drawFace(int p0, int p1, int p2, int p3);

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
  handleInputs();
  
  updateCube();
  renderCube();

  delay(5);
}

//      --- Functions ---

// Handles the inputs from the potentiometer and the buttons
void handleInputs() {
  int potentiometer_value = analogRead(PIN_POTENTIOMETER);

  // I think 50 to 250 is a good range for the cube's distance. If its too low it suffers from distortion,
  // and if its too high, well, its too far to the screen to see.
  distance = map(potentiometer_value, 0, 4095, 50, 250);

  if (checkButtons(PIN_BUTTON_INVERT, last_button_state_invert)) {
    invert = !invert;
  }

  if (checkButtons(PIN_BUTTON_SOLID, last_button_state_solid)) {
    solid = !solid;
  }
}

// Manages a button toggle system by checking its current and last state.
bool checkButtons(int pin, int &last_button_state) {
  int current_button_state = digitalRead(pin);

  // Only true if it was previously HIGH, and is now LOW. As the last state changes immediately after the button
  // is pressed, the condition can not be true until you unpress the button and press it again.
  bool pressed = (last_button_state == HIGH && current_button_state == LOW);
  
  last_button_state = current_button_state;

  return pressed;
}

// Updates the cube's angle using rotation matrices, and projects the results onto the 2D space,
// so it can be shown on the screen.
void updateCube() {
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

    // Projects in perspective the 3d vectors onto the 2D space.
    vertices2D[i][0] = (int)(x1 * 60 / (z2 + distance)) + 64;
    vertices2D[i][1] = (int)(y2 * 60 / (z2 + distance)) + 32;
  }

  // Modifying the angles. don't know why, but if both are the same value the animation
  // comes out laggy in a point. More like a little jump.
  angle_x += 0.03;
  angle_y += 0.02;
}

// Draws the cube on the screen using the already processed vertices given by the updateCube() function.
void renderCube() {
  display.invertDisplay(invert);
  display.clearDisplay();
  
  if (solid) {
    for (int j = 0; j < 6; j++) {
      // Takes the vertices forming the actual face.
      int p0 = faces[j][0];
      int p1 = faces[j][1];
      int p2 = faces[j][2];
      int p3 = faces[j][3];
      
      if (visible(p0, p1, p2)) {
        drawFace(p0, p1, p2, p3);
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
}

// Checks if a face is visible by evaluating the cross point of two vectors. These vectors are formed by
// three consecutive vertices from one of the cube's faces.
bool isFaceVisible(int vertex_0, int vertex_1, int vertex_2) {
  // Takes three consecutive vertices, and draws the vectors from the first one
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

// Draws the cube's faces if they are visible. This function only executes if the solid mode is activated.
void drawFace(int p0, int p1, int p2, int p3) {
  display.drawLine(vertices2D[p0][0], vertices2D[p0][1], vertices2D[p1][0], vertices2D[p1][1], WHITE);
  display.drawLine(vertices2D[p1][0], vertices2D[p1][1], vertices2D[p2][0], vertices2D[p2][1], WHITE);
  display.drawLine(vertices2D[p2][0], vertices2D[p2][1], vertices2D[p3][0], vertices2D[p3][1], WHITE);
  display.drawLine(vertices2D[p3][0], vertices2D[p3][1], vertices2D[p0][0], vertices2D[p0][1], WHITE);
}
