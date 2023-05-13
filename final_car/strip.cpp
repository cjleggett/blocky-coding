#include "strip.h"
#include <Arduino.h>
#include "constants.h"
#include <FastLED.h>

Strip :: Strip(CRGB *leds_in)
{
  leds = leds_in;
  on = false;
  mode = 1;
  last_time = millis();
  buffer = 1000;

}

// Sets all LEDs to a certain color
void Strip :: update_all(int c)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = colors[c];
  }
}

// Switches state of all lights on or off
void Strip :: blink_update(long duration)
{
  // switch from on to off
  if (millis() - last_time > duration)
  {
    on = !on;
    last_time = millis();
  }

  if (on)
  {
    update_all(color);
  }
  else
  {
    update_all(3);
  }
}

// Turns one light on at a time in a circle
void Strip :: circle_update(long duration)
{
  long time = millis() - last_time;
  // reset time if surpassed to avoid overflow
  if (time > duration){
    last_time = millis();
    time = 0;
  }
  for (int i = 0; i < NUM_LEDS; i ++)
  {
    if (time * NUM_LEDS / duration == i) 
    {
      leds[i] = colors[color];
    }
    else
    {
      leds[i] = colors[3];
    }
  }
  
}

// Based on state, update all LEDs
void Strip :: update()
{
  if (mode == 1) {
    update_all(color);
  }
  if (mode == 0) {
    update_all(3);
  }
  if (mode == 2){
    blink_update(500);
  }
  if (mode == 3){
    blink_update(250);
  }
  if (mode == 4) {
    circle_update(1000);
  }
  FastLED.show();
}

// 0 = off, 1 = constant, 2 = blink, 3 = blink fast, 4 = circle
void Strip :: change_mode()
{
  mode = (mode + 1) % 5;
}

// 0 = red, 1 = blue, 2 = green
void Strip :: change_color()
{
  color = (color + 1) % 3;
}

void Strip :: off()
{
  mode = 0;
  color = 0;
}