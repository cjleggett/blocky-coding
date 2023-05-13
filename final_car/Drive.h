#ifndef DRIVE_H
#define DRIVE_H
#include <Arduino.h>

class Drive {
  private:
    int motor1A;
    int motor1B;
    int motor2A;
    int motor2B;

    int freq;
    int channel1;
    int channel2;
    int resolution;
    int speed;

    int direction;
    long duration;
    long prevTime;
    long interval;

    // 0: stop, 1: forward, 2: turn right, 2: turn left
    int state;
    void rightWheel(bool forward);
    void leftWheel(bool forward);
    

  public:
    Drive();
    void setup();
    void forward();
    void backward();
    void retreat();
    void stop();
    void turn(int right);
    void turnRandom();
    void update();
};

#endif