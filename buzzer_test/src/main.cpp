#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define width 128
#define height 64

Adafruit_SSD1306 display(width, height, &Wire, -1);

enum notes {
  C = 262, CS = 277, D = 294, DS = 312, E = 330, F = 350,
  FS = 370, G = 392, GS = 416, A = 440, AS = 466, B = 494,
  REST = 0
};

void music();

// I made some things for making music easier. Now i just have to write the melody, the
// length of every note, it's octave and, just if I want, lyrics. Also, I can decide
// if I want the music to repeat or not.
const int tempo = 120;
const int beat = 60000 / tempo;

const int melody[] = {C, G, B, FS, C, G, B, FS, C, G, B, FS, C, G, B, FS, 
                      G, G, FS, F, G, FS, E, B, E, CS, B, E,
                      G, G, FS, F, B, G, FS, B, E, C, B, E,
                      G, G, FS, F, G, FS, E, B, E, CS, B, E,
                      G, G, FS, F, B, G, FS, B, C, C,
                      C, G, B, FS, C, G, B, FS, C, G, B, FS, E, C, B, E
                      };

// I want every array to have the same length.
const int total_notes = sizeof(melody) / sizeof(melody[0]);
                
const float lengths[total_notes] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                    2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                    2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                    2, 2, 2, 2, 1, 1, 1, 1, 2, 2,
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const int octaves[total_notes] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4,
                                  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4,
                                  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4,
                                  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 3, 4};

const char* lyrics[total_notes] = {"Lavender town"};

const char repeat = 'y';

const int pin_buzzer = 18;
const int channel = 0;

void setup() {

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while(true);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  ledcSetup(channel, 2000, 8);
  ledcAttachPin(pin_buzzer, channel);

  // Plays at least one time.
  music(); 
}

void loop() {

  // and if I want it to repeat, it does.
  if (repeat == 'y') music();
}

void music() {

  for (int i = 0; i < total_notes; i++) {

    // Stops the program if the length or octave is not valid.
    if (octaves[i] < 0 || lengths[i] < 0) {

      while (true) delay(1000);
    }

    // minus 4 because I made 4 the central octave.
    int note = melody[i] * pow(2, octaves[i] - 4);

    float length = beat * lengths[i];

    // "" if you dont want to change the lyrics, and nullptr in the case
    // I just want a banner.
    if (lyrics[i] != "" && lyrics[i] != nullptr) {

      display.clearDisplay();
      display.setCursor(10, 20);
      display.print(lyrics[i]);
      display.display();
    }
    
    ledcWriteTone(channel, note);
    delay(length * 0.8);

    // 20% of every note is a silence. Else the notes sounds too close I think.
    ledcWriteTone(channel, 0);
    delay(length * 0.2);
  }
}