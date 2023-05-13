#include "Drive.h"
#include <Arduino.h>

Drive :: Drive() {
  motor1A = 18;
  motor1B = 23;
  motor2A = 10;
  motor2B = 5;

  freq = 5000;
  channel1 = 1;
  channel2 = 2;
  resolution = 8;
  speed = 255;

  pinMode(channel1, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(channel2, OUTPUT);
  pinMode(motor2B, OUTPUT);

  interval = 100;
}

void Drive :: setup() {
  ledcSetup(channel1, freq, resolution);
  ledcAttachPin(motor1A, channel1);

  ledcSetup(channel2, freq, resolution);
  ledcAttachPin(motor2A, channel2);
  stop();
}

void Drive :: forward() {
  if (state == 0) {
    state = 1;
  }
}

void Drive :: backward() {
  state = 0;
  ledcWrite(channel1, 255 - speed);
  digitalWrite(motor1B, HIGH);
  ledcWrite(channel2, speed);
  digitalWrite(motor2B, LOW);
}

// Turning in only one direction
// This should be a wiring problem though rather than code
void Drive :: turn(int right) {
  state = 0;
  if (right) {
    Serial.println("RIGHT");
    ledcWrite(channel1, 255 - speed);
    digitalWrite(motor1B, HIGH);
    ledcWrite(channel2, 255 - speed);
    digitalWrite(motor2B, HIGH);
  } else {
    Serial.println("LEFT");
    ledcWrite(channel1, speed);
    digitalWrite(motor1B, LOW);
    ledcWrite(channel2, speed);
    digitalWrite(motor2B, LOW);
  }
}

void Drive :: stop() {
  state = 0;
  ledcWrite(channel1, 255 - speed);
  digitalWrite(motor1B, LOW);
  ledcWrite(channel2, 0);
  digitalWrite(motor2B, 0);
}

void Drive :: turnRandom() {
  state = 0;
  randomSeed(millis());
  duration = random(300, 900);
  direction = 1;
  turn(direction);
  delay(duration);
  stop();
  delay(500);
}

void Drive :: update() {
  // Driving forward only 3/10 of the time to slow down
  if (state == 0) {
    return;
  }
  if (millis() - prevTime > interval) {
    state = 1 + (state % 10);
    prevTime = millis();
  }
  if (state == 1 || state == 4 || state == 7 ) {
    ledcWrite(channel1, speed);
    digitalWrite(motor1B, LOW);
    ledcWrite(channel2, 255 - speed);
    digitalWrite(motor2B, HIGH);
  }
  else {
    ledcWrite(channel1, 255 - speed);
    digitalWrite(motor1B, LOW);
    ledcWrite(channel2, 0);
    digitalWrite(motor2B, 0);
  }
}