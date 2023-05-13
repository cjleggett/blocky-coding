#ifndef COLORSENSOR_H
#define COLORSENSOR_H
#include "Adafruit_TCS34725.h"
#include <Arduino.h>

class ColorSensor {
  private:
    Adafruit_TCS34725 tcs;
    uint16_t r, g, b, c, colorTemp, lux;
    int rgbc[4];
    void senseColors();

  public:
    ColorSensor();
    void setup();
    void getColors(int *buffer);
    String getColorName();
};

#endif