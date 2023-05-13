#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

class Buzzer
{
  private:
    int buzzerPin;
    int duration;
    long lastTime = 0;
    int frequency = 0; 

  public:
    Buzzer(int buzzerPinIn);
    void playNote(float newFrequency, long newDuration);
    void playUploaded();
    void update();
    void off();
};

#endif