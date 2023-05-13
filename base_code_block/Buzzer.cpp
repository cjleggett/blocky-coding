#include "Buzzer.h"
#include <Arduino.h>
#include "notes.h"

float notes[5] = {C4, E4, G4, E4, C4};

Buzzer :: Buzzer(int buzzerPinIn)
{
  buzzerPin = buzzerPinIn;
  pinMode(buzzerPin, OUTPUT);
}

// Plays note at specified frrequency for specified length
void Buzzer :: playNote(float newFrequency, long newDuration)
{
  tone(buzzerPin, newFrequency);
}

// Hard resets the buzzer to off
void Buzzer :: off() {
  noTone(buzzerPin);
  duration = 0;
}

// Plays thirds CEGEC to indicate upload
void Buzzer :: playUploaded() {
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, notes[i]);
    delay(300);
    noTone(buzzerPin);
    delay(50);
  }
}

void Buzzer :: update()
{
  if (millis() - lastTime > duration) {
    noTone(buzzerPin);
  }
}