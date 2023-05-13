#include "ColorSensor.h"
#include <Arduino.h>

// Lower and upper bounds for acrylic green, yellow, and blue;
const int lowers[3][4] = {
  {580, 840, 440, 2150},
  {0, 0, 0, 2500},
  {550, 850, 500, 2100}
};

const int uppers[3][4] = {
  {650, 950, 490, 2200},
  {10000, 10000, 10000, 10000},
  {650, 950, 600, 2400}
};
const String colors[3] = {"Green", "Yellow", "Blue"};


ColorSensor :: ColorSensor() {
  tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
}

void ColorSensor :: setup() {
  while (!tcs.begin()) {
    Serial.println("No TCS34725 found ...");
  }
  Serial.println("Found sensor!");
}

// Returns "Yellow", "Blue", "Green", or "Other"
String ColorSensor :: getColorName() {
  senseColors();
  for (int k = 0; k < 3; k++) {
    bool color = true;
    for (int i = 0; i < 4; i++) {
      if (rgbc[i] > uppers[k][i] || rgbc[i] < lowers[k][i]) {
        color = false;
      }
    }
    if (color) {
      return colors[k];
    }
  }
  return "Other";
}

// Senses colors quickly
void ColorSensor :: senseColors() {
  rgbc[3] = tcs.read16(TCS34725_CDATAL);
  rgbc[0] = tcs.read16(TCS34725_RDATAL);
  rgbc[1] = tcs.read16(TCS34725_GDATAL);
  rgbc[2] = tcs.read16(TCS34725_BDATAL);
}

// Adds colors to buffer
void ColorSensor :: getColors(int *buffer) {
  senseColors();
  for (int i = 0; i < 4; i++) {
    buffer[i] = rgbc[i];
  }
}