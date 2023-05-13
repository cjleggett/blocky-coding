#include "Buzzer.h"
#include <Arduino.h>

const int durations[4] = {300, 50, 600, 500};


Buzzer :: Buzzer(int p, int n) {
  pin = p;
  note = n;
  state = 0;
  lastTime = millis();
}

void Buzzer :: setup() {
  ledcSetup(channel, note, resolution);
  ledcAttachPin(pin, channel);
}

// Using delay due to problems with drifting off the board while driving
void Buzzer :: beep() {
  ledcWriteTone(channel, note);
  delay(durations[0]);
  ledcWriteTone(channel, 0);
  delay(durations[1]);
  ledcWriteTone(channel, note);
  delay(durations[2]);
  ledcWriteTone(channel, 0);
}

// Turn off
void Buzzer :: off() {
  ledcWriteTone(channel, 0);
}

// Currently not used, but should eventually replace the beep functionality
void Buzzer :: update() {
  newTime = millis();
  if (state == 0) {
    off();
  }
  else if (state == 1 && newTime - lastTime > durations[0]) {
    state = 2;
    off();
    lastTime = millis();
  }
  else if (state == 2 && newTime - lastTime > durations[1]) {
    state = 3;
    ledcWriteTone(channel, note);
    lastTime = millis();
  }
  else if (state == 3 && newTime - lastTime > durations[2]) {
    state = 4;
    off();
    lastTime = millis();
  } else if (state == 4 && newTime - lastTime > durations[3]) {
    state = 0;
  }
}