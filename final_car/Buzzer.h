#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

class Buzzer {
  private:
    int note;
    int pin;
    long lastTime;
    long newTime;
    int state;
    int channel = 3;
    int resolution = 8;
    void off();

  public:
    Buzzer(int p, int n);
    void beep();
    void update();
    void setup();
};

#endif