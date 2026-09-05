#include <Arduino.h>

const int light_sensor = 23;
const int led_pin = 18;

int stable_state = LOW;
int last_state = LOW;
unsigned long last_change = 0;
const int filter = 500;

void setup()
{

  Serial.begin(115200);
  pinMode(light_sensor, INPUT);
  pinMode(led_pin, OUTPUT);
}

void loop()
{

  int actual_state = digitalRead(light_sensor);

  // If the state changes, it records the exact time it happened
  if (actual_state != last_state)
  {
    last_change = millis();
  }

  // A signal is valid if it's stable for at least 500 milliseconds.
  if ((millis() - last_change) > filter)
  {

    // if it's not the previous stable state, the actual state is the new stable
    // state, and for more satisfacton it turns on a led, color of your preference.
    if (actual_state != stable_state)
    {

      stable_state = actual_state;

      if (stable_state == HIGH)
      {

        digitalWrite(led_pin, HIGH);
      }

      else
      {

        digitalWrite(led_pin, LOW);
      }
    }
  }

  last_state = actual_state;
}
