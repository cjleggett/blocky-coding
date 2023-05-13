#ifndef STRIP_H
#define STRIP_H
#include <Arduino.h>
#include "constants.h"
#include <FastLED.h>

class Strip
{
  private:
    long last_time;
    long last_time_2;
    long buffer;
    // 0 = red, 1 = blue, 2 = green
    int color;
    // 0 = off, 1 = constant, 2 = blink, 3 = blink fast, 4 = circle
    int mode;
    CRGB *leds;
    CRGB colors[4] = {CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::Black};
    bool on;
    void update_all(int color);
    void blink_update(long duration);
    void circle_update(long duration);



  public:
    Strip(CRGB *leds_in);
    void update();
    void change_color();
    void change_mode();
    void off();
};

#endif